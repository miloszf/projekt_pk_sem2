#include "simulator.h"
#include "cpu.h"
#include "terminal.h"
#include "window.h"
#include "graphics.h"
#include "error.h"
#include "settings.h"

struct Simulator
{
	struct Terminal* terminal;
	struct Window* window;
	struct CPU* cpu;
	struct Console* console;
};

struct Simulator* simulator_init(int argc, char** argv)
{
	init_crash_log();
	struct Simulator* new_sim = malloc_s(sizeof(struct Simulator));
	*new_sim = (struct Simulator){ NULL, NULL, NULL };

	new_sim->terminal = terminal_init("MaszynaW");
	new_sim->window = window_init();
	struct Canvas* cpu_canvas = window_new_canvas(new_sim->window, (Point) { 0, 0 }, (Point) { 73, 31 });
	struct Canvas* console_canvas = window_new_canvas(new_sim->window, (Point) { 0, 31 }, (Point) { 73, 40 });
	new_sim->cpu = cpu_init(cpu_canvas);
	new_sim->console = console_init(cpu_canvas);

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