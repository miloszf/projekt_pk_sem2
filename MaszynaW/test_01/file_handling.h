#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include "error.h"

char** get_lines(const char* file_name);
Error compile_instruction_list(const char* file_name);

#endif
