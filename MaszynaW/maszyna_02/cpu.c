#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cpu.h"
#include "vector.h"
#include "error.h"
#include "settings.h"
#include "cpu_auxiliary_structs.h"




struct CPU
{
	struct CPUSetup setup;
	struct CPUComponents components;
	struct CPUMemory memory;
	struct
	{
		struct Vector* units;
		struct Vector* signals;
		struct Vector* tags;
	} vector;
};


void cpu_init_basic(struct CPUComponents* components, struct Vector* unit_vect, struct Vector* signal_vect)
{
	check_for_NULL(components);

	// basic ALU
	//{
	//	Point alu_offset = {}
	//}
}



struct CPU* cpu_init()
{
	struct CPU* cpu = malloc_s(sizeof(struct CPU));
	//cpu->setup = setup;

	//struct CPUPreference setup[] = {
	struct CPUSetup setup = { 
		.preference.list = {
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
			cpu_preference_init("wyjscie"),
			cpu_preference_init("znaczniki")},
		.basic = {
			.unit_vect = vector_init(sizeof(struct Unit*)),
			.signal_vect = vector_init(sizeof(struct Signal*))}
	};

	struct CPUComponents components;

	/* inicjalizacja unitów */ 
	// ...

	/* inicjalizacja sygna³ów */
	// ...

	/* za³adowanie ustawieñ z pliku */
	// ...

	/* inicjalizacja cpu zgodnie z ustawieniami 
	   utworzenie wektora obs³ugiwanych */
	// ...


	return cpu;
}


void cpu_delete(struct CPU* cpu)
{
	if (cpu)
	{
		cpu_setup_delete(&cpu->setup);
		free(cpu);
	}
}