#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

#define LOG (struct CrashLog) { __FILE__, __func__, __LINE__ }
#define CRASH_LOG(x) do {_crash_log(__FILE__, __func__, __LINE__, x); exit(-1);} while(0)
#define CHECK_IF_NULL(x) do {if (!(x)) { _crash_log(__FILE__, __func__, __LINE__, NULL_DEREFERENCE); exit(-1); }} while(0)

typedef enum { NULL_DEREFERENCE, LIBRARY_FUNC_FAILURE, MEM_ALOC_FAILURE, LOG_UNKNOWN_VALUE, GRAPHICS_FAILURE, TERMINAL_FAILURE } ProgramErrorType;
struct CrashLog
{
	const char* file;
	const char* function;
	int line;
};

void init_crash_log();
void _crash_log(const char* file, const char* func, int line, ProgramErrorType type);

typedef int Error;
typedef enum { NO_FILE = 1, MISSING_INPUT, INVALID_INPUT, MISSING_LINE, UNKNOWN_LABEL, REPEATED_LABEL, LOST_TICK } CompilationError;
typedef enum { CPU_STOPPED = 1, UNKNOWN_INSTRUCTION, ALREADY_SET, EMPTY_UNIT } RuntimeError;


Error error();
Error error_msg(const char** message);
void instr_error_set(CompilationError error, const char* arg);
void prog_error_set(CompilationError error, const char* arg);
void runtime_error_set(RuntimeError error, const char* arg);

void* malloc_s(size_t size);
void* calloc_s(size_t count, size_t size);
void* realloc_s(void* ptr, size_t new_size);

#endif