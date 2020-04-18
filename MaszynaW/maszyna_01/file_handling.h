#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include "error.h"
#include "cpu.h"

//struct Instruction

unsigned char** get_lines(const char* file_name);
Error get_settings(unsigned char** list, struct CPUSetup* setup);
char** get_instr_names(char** list, int* instr_num);
Error compile_instructions(const char* file_name, struct CPU* cpu);

#endif
