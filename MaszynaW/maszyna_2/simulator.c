#include "simulator.h"
#include "print_error.h"


#include <stdlib.h>

struct Simulator
{
	Error error;
};

struct Simulator* sim_init(int argc, const char** argv)
{
	struct Simulator* sim = malloc(sizeof(struct Simulator));
	Error error = NO_ERROR;

	if (sim)
	{


		sim->error = error;
	}
	else
		error |= ERROR_NULL_POINTER;

	print_error(error);
	return sim;
}

void sim_run(struct Simulator* sim)
{
	if (!sim)
	{

	}
}

void sim_delete(struct Simulator* sim)
{
	free(sim);
}