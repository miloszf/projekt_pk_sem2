#include <stdio.h>
#include "simulator.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main(int argc, char** argv)
{
	if (argc >= 3)
	{
		const char* instr_file_name = argv[1];
		const char* prog_file_name = argv[2];

		simulator_run(instr_file_name, prog_file_name);
	}
	else
		printf("usage: [program_name] [instructions_file] [program_file]\n");
	
	_CrtDumpMemoryLeaks();
	return 0;
}
