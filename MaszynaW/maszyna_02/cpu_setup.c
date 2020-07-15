#include <stdlib.h>
#include <string.h>

#include "cpu_setup.h"
#include "cpu_structs.h"
#include "error.h"
#include "vector.h"
#include "unit.h"
#include "signal.h"

struct CPUPreference cpu_preference_init(const char* name)
{
	CHECK_IF_NULL(name);
	char* new_name = _strdup(name);
	if (!new_name)
		CRASH_LOG(LIBRARY_FUNC_FAILURE);
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
		{
			unit_delete(*unit_ptr);
			free(unit_ptr);
		}
		vector_delete(pref->unit_vect);
		struct Signal** signal_ptr;
		while (signal_ptr = vector_pop(pref->signal_vect))
		{
			signal_delete(*signal_ptr);
			free(signal_ptr);
		}
		vector_delete(pref->signal_vect);
	}
}

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