#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cpu_auxiliary_structs.h"
#include "error.h"
#include "settings.h"
#include "vector.h"
#include "unit.h"
#include "signal.h"

struct CPUPreference cpu_preference_init(const char* name)
{
	check_for_NULL(name);
	// strdup?
	size_t name_len = strlen(name) + 1;
	char* new_name = malloc_s(name_len);
	if (strcpy_s(new_name, name_len, name))
		critical_error_set("strcpy_s failed\n");
	// ?
	struct Vector* unit_vect = vector_init(sizeof(struct Unit*));
	struct Vector* signal_vect = vector_init(sizeof(struct Signal*));
	return (struct CPUPreference) { new_name, 0, unit_vect, signal_vect };
}

void cpu_preference_delete(struct CPUPreference* pref)
{
	if (pref)
	{
		free((char*)pref->name);
		struct Unit** unit_ptr;
		while (unit_ptr = vector_pop(pref->unit_vect))
			unit_delete(*unit_ptr);
		vector_delete(pref->unit_vect);
		struct Signal** signal_ptr;
		while (signal_ptr = vector_pop(pref->signal_vect))
			signal_delete(*signal_ptr);
		vector_delete(pref->signal_vect);
	}
}

// -----

struct CPUSetup cpu_setup_init()
{
	struct CPUSetup setup = {
		.list = {
			cpu_preference_init("adres"),
			cpu_preference_init("kod"),
			cpu_preference_init("polaczenie"),
			cpu_preference_init("inkrementacja"),
			cpu_preference_init("logiczne"),
			cpu_preference_init("arytmetyczne"),
			cpu_preference_init("stos"),
			cpu_preference_init("rejestrx"),
			cpu_preference_init("rejestry"),
			cpu_preference_init("przerwania"),
			cpu_preference_init("wejscie"),
			cpu_preference_init("znaczniki"),
			cpu_preference_init(""),
		}
	};
	setup.all.addr_length.value = 5;
	setup.all.code_length.value = 3;
	setup.all.basic.value = 1;
	return setup;
}

void cpu_setup_delete(struct CPUSetup* setup)
{
	if (setup)
	{
		for (int i = 0; i < CPU_SETUP_SIZE; i++)
			cpu_preference_delete(setup->list + i);
	}
}

// -----

struct CPUTag cpu_tag_init(const char* name)
{
	check_for_NULL(name);
	char* new_name = _strdup(name);
	if (!new_name)
		critical_error_set("strdup failed\n");
	return (struct CPUTag) { new_name, false };
}

void cpu_tag_delete(struct CPUTag* tag)
{
	if (tag)
	{
		free((char*)tag->name);
	}
}