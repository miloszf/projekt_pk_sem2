#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdbool.h>
//#include "error.h"
//#include "cpu.h"
#include "signal.h"
//#include "settings.h"

//struct InstructionsNames;
//struct InstructionsSignals;

struct Instruction
{
	struct Tick
	{
		//struct InstructionList* next;
		struct Signal* signals;
		struct Conditional {
			struct Tick* if_true;
			struct Tick* if_false;
			bool* condition;
		} *next;
	};
};

//Error instructions_get_setup(const char* file_name, struct CPUSetup* setup);
//Error instructions_get_names(const char* file_name, struct InstructionsNames** instr_names);
//Error instructions_get_signals(const char* file_name, struct InstructionsNames** instr_names, struct InstructionsSignals** instr_signals);
//Error instructions_compile(struct InstructionsNames** names, struct InstructionList** list);

//Error instructions_get_names(struct InstructionList** list, struct InstructionsNames* names, struct CPUTags* tags);

#endif