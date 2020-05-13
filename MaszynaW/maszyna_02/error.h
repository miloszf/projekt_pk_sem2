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


//#define
//typedef enum { INSTR_COMP_ERROR = 1, PROG_COMP_ERROR, RUNTIME_ERROR } UserErrorType;
//typedef enum { NO_FILE = 1, MISSING_INPUT, INVALID_INPUT, MISSING_LINE, UNKNOWN_LABEL, REPEATED_LABEL, LOST_TICK } CompilationError;
//typedef enum { CPU_STOPPED = 1, UNKNOWN_INSTRUCTION, ALREADY_SET, EMPTY_UNIT } RuntimeError;
typedef int Error;

typedef Error UserErrorType;
#define USER_ERROR_TYPE  0xE000
#define INSTR_COMP_ERROR 0x8000
#define PROG_COMP_ERROR	 0x4000
#define RUNTIME_ERROR	 0x2000

typedef Error CompilationError;
#define NO_FILE				0x0001
#define MISSING_INPUT		0x0002
#define INVALID_INPUT		0x0004
#define MISSING_LINE		0x0008
#define UNKNOWN_LABEL		0x0010
#define REPEATED_LABEL		0x0020
#define LOST_TICK			0x0040

typedef Error RuntimeError;
#define CPU_STOPPED			0x0001
#define UNKNOWN_INSTRUCTION	0x0002
#define ALREADY_SET			0x0004
#define EMPTY_UNIT			0x0008
#define INVALID_IO_ADDRESS	0x0010

Error error();
const char* error_msg();
void instr_error_set(CompilationError error, const char* arg);
void prog_error_set(CompilationError error, const char* arg);
void runtime_error_set(RuntimeError error, const char* arg);
void error_reset();

void* malloc_s(size_t size);
void* calloc_s(size_t count, size_t size);
void* realloc_s(void* ptr, size_t new_size);

#endif