#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"

#define EXIT_VALUE -10
#define CRASH_MSG_LENGTH 256

char crash_log_msg[CRASH_MSG_LENGTH] = "";

struct ErrorStruct {
	Error err;
	const char* msg;
};

unsigned log2(unsigned num)
{
	unsigned log = 0;
	while (num = num >> 1)
		log++;
	return log;
}

struct ErrorStruct* get_error()
{
	static struct ErrorStruct* s_error = NULL;
	if (s_error)
		return s_error;
	else
	{
		s_error = malloc(sizeof(struct ErrorStruct));
		if (s_error)
			*s_error = (struct ErrorStruct){ 0, NULL };
		return s_error;
	}
}

void log_at_exit(void)
{
	printf("%s", crash_log_msg);
	free(get_error());
}

void init_crash_log()
{
	static bool is_set = false;
	if (!is_set)
	{
		if (atexit(&log_at_exit))
			exit(EXIT_VALUE);
		is_set = true;
	}
}

void _crash_log(const char* file, const char* func, int line, ProgramErrorType type)
{
	const char* msg_array[] = {
		" - Null pointer dereference\n",
		" - library function failed\n",
		" - memory allocation error\n",
		" - invalid value, possibly in switch statement\n",
		" - graphics failure\n",
		" - Windows terminal handling failure\n"
	};
	const char* msg;
	if (type < (sizeof(msg_array) / sizeof(char*)))
		msg = msg_array[type];
	else
		CRASH_LOG(LOG_UNKNOWN_VALUE);
	int unused = snprintf(crash_log_msg, CRASH_MSG_LENGTH, "Critical error!%sfile: %s\nfunction: %s - line(%d)\n", msg, file, func, line);
}

Error error()
{
	struct ErrorStruct* error = get_error();
	CHECK_IF_NULL(error);
	return error->err;
}

const char* error_msg()
{
	struct ErrorStruct* error = get_error();
	CHECK_IF_NULL(error);
	return error->msg;
}

void error_set(UserErrorType err_type, Error error, const char* arg)
{
	struct ErrorStruct* error_s = get_error();
	CHECK_IF_NULL(error_s);

	const char* instr_err_str = ">Instruction file compilation error:";
	const char* prog_err_str = ">Program file compilation error:";
	const char* runtime_err_str = ">Runtime error:";
	const char* comp_format_array[] = {
		"%s cannot access \"%s\" file\n",
		"%s cannot find \"%s\"\n",
		"%s invalid value \"%s\"\n",
		"%s missing line in section \"%s\"\n",
		"%s unknown label \"%s\"\n",
		"%s repated label \"%s\"\n",
		"%s instruction \"%s\" has a line which won't be reached\n",
		"%s not enough memory for a program, increase number of address bits\n"
	};
	const char* runtime_format_array[] = {
		">Program finished\n",
		"%s unknown instruction\n",
		"%s execution failed - output already set\n",
		"%s execution failed - input in not set\n",
		"%s invalid io address\n",
	};
	const char empty[] = "";
	if (!arg)
		arg = empty;

	const char* header = NULL;
	const char* format = NULL;

	switch (err_type)
	{
	case INSTR_COMP_ERROR:
	case PROG_COMP_ERROR:
		header = instr_err_str;
		break;
	case RUNTIME_ERROR:
	{
		header = instr_err_str;
		unsigned index = log2(error);
		if (index < (sizeof(runtime_format_array) / sizeof(char*)))
			format = runtime_format_array[index];
		else
			CRASH_LOG(LOG_UNKNOWN_VALUE);
	}
	break;
	default:
		CRASH_LOG(LOG_UNKNOWN_VALUE);
		break;
	}

	if (err_type == INSTR_COMP_ERROR || err_type == PROG_COMP_ERROR)
	{
		unsigned index = log2(error);
		if (index < (sizeof(comp_format_array) / sizeof(char*)))
			format = comp_format_array[index];
		else
			CRASH_LOG(LOG_UNKNOWN_VALUE);
	}

	size_t new_msg_len = strlen(format) + strlen(header) + strlen(arg) + 1;
	char* new_msg = malloc_s(new_msg_len);
	if (sprintf_s(new_msg, new_msg_len, format, header, arg) <= 0)
		CRASH_LOG(LIBRARY_FUNC_FAILURE);
	if (error_s->msg)
		free((char*)error_s->msg);
	*error_s = (struct ErrorStruct){ (err_type | error), new_msg };
}

void instr_error_set(CompilationError error, const char* arg)
{
	error_set(INSTR_COMP_ERROR, error, arg);
}

void prog_error_set(CompilationError error, const char* arg)
{
	error_set(PROG_COMP_ERROR, error, arg);
}

void runtime_error_set(RuntimeError error, const char* arg)
{
	error_set(RUNTIME_ERROR, error, arg);
}

void error_reset()
{
	struct ErrorStruct* error = get_error();
	CHECK_IF_NULL(error);
	free((char*)error->msg);
	*error = (struct ErrorStruct){ 0, NULL };
}

void* malloc_s(size_t size)
{
	void* value_ptr = malloc(size);
	if (!value_ptr)
		CRASH_LOG(MEM_ALOC_FAILURE);
	return value_ptr;
}

void* calloc_s(size_t count, size_t size)
{
	void* value_ptr = calloc(count, size);
	if (!value_ptr)
		CRASH_LOG(MEM_ALOC_FAILURE);
	return value_ptr;
}

void* realloc_s(void* ptr, size_t new_size)
{
	void* value_ptr = realloc(ptr, new_size);
	if (!value_ptr)
		CRASH_LOG(MEM_ALOC_FAILURE);
	return value_ptr;
}
