#include <stdlib.h>
#include <locale.h>

#include "simulator.h"
#include "print_error.h"
#include "cpu.h"


struct Simulator
{
	Error error;
	struct CPU* cpu;
};

struct Simulator* sim_init(int argc, const char** argv)
{
	struct Simulator* sim = malloc(sizeof(struct Simulator));
	Error error;

	if (sim)
	{
		setlocale(LC_ALL, "Polish_Poland.1250");
		error = cpu_init(&sim->cpu);


		sim->error = error;
	}
	else
		error = ERROR_NULL_POINTER;

	print_error(&error);
	return sim;
}

void sim_run(struct Simulator* sim)
{
	if (sim)
	{
		cpu_run(sim->cpu);
	}
}

void sim_delete(struct Simulator* sim)
{
	free(sim);
}