#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

struct CPU;

struct CPUSetup
{
	bool as_link;
	bool alu_inc;
	bool alu_logic;
	bool alu_ext;
	bool stack;
	bool reg_x;
	bool reg_y;
	bool intrp;
	bool io;
	bool tags;
};

struct CPU* cpu_init(struct CPUSetup* cu_setup);
void cpu_delete(struct CPU* cu);

#endif

