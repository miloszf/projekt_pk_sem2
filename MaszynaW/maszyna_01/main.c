#include "simulator.h"

int main()
{
	const char* argv[] = { "nazwa programu" };
	int argc = sizeof(argv) / sizeof(argv[0]);

	struct Simulator* sim = sim_init(argc, argv);
	if (sim)
		sim_run(sim);

	return 0;
}