#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include "error.h"

struct CPU;

struct CPUSetup
{
	unsigned char address_length;
	unsigned char code_length;
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

struct CPUTags
{
	bool Z;
	bool V;
	bool INT;
	bool ZAK;
};

Error cpu_init(struct CPU** cpu_ptr);
void cpu_delete(struct CPU* cpu);
Error cpu_run(struct CPU* cpu);

struct CPUSetup* cpu_get_setup(struct CPU* cpu);
struct CPUTags* cpu_get_tags(struct CPU* cpu);
struct Signal* cpu_get_signals(struct CPU* cpu);

#endif

