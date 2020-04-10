#ifndef INSTRUCTIONS
#define INSTRUCTIONS

#include "unit.h"

struct Instruction
{
	char* name;
	struct Unit* input;
	struct Unit* output;
};

#endif