#include "simulator.h"
#include "cpu.h"
#include "terminal.h"
#include "window.h"
#include "graphics.h"
#include "error.h"

struct Simulator
{
	struct CPU* cpu;
	struct Terminal* terminal;
	struct Window* window;
};

struct Simulator* simulator_init(int argc, char** argv)
{
	struct Simulator* new_sim = malloc_s(sizeof(struct Simulator));
	*new_sim = (struct Simulator){ NULL, NULL, NULL };


	return new_sim;
}

void simulator_delete(struct Simulator* sim)
{
	if (sim)
	{

	}
}

void simulator_run(struct Simulator* sim)
{

}