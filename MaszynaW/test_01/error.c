#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"

#define EXIT_VALUE -10
#define CRASH_MSG_LENGTH 256

typedef enum { INSTR_COMP_ERROR = 1, PROG_COMP_ERROR, RUNTIME_ERROR } UserErrorType;
struct ErrorStruct {
	Error err;
	const char* msg;
};

char crash_log_msg[CRASH_MSG_LENGTH] = "";

struct ErrorStruct* get_error()
{
	static struct ErrorStruct* s_error = NULL;
	if (s_error)
		return s_error;
	else
	{
		s_error = malloc(sizeof(struct ErrorStruct));
		if (s_error)
		{
			*s_error = (struct ErrorStruct){ 0, NULL };
		}
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
		" - Windows terminal handling failure\n",
		"\n"
	};
	const char* msg;
	switch (type)
	{
	case NULL_DEREFERENCE:
		msg = msg_array[0];
		break;
	case LIBRARY_FUNC_FAILURE:
		msg = msg_array[1];
		break;
	case MEM_ALOC_FAILURE:
		msg = msg_array[2];
		break;
	case LOG_UNKNOWN_VALUE:
		msg = msg_array[3];
		break;
	case GRAPHICS_FAILURE:
		msg = msg_array[4];
		break;
	case TERMINAL_FAILURE:
		msg = msg_array[5];
		break;
	default:
		msg = msg_array[6];
		break;
	}
	int unused = snprintf(crash_log_msg, CRASH_MSG_LENGTH, "Critical error!%sfile: %s\nfunction: %s - line(%d)\n", msg, file, func, line);
	//exit(EXIT_VALUE);
}

Error error()
{
	struct ErrorStruct* error = get_error();
	CHECK_IF_NULL(error);
	return error->err;
}

Error error_msg(const char** message)
{
	CHECK_IF_NULL(message);
	struct ErrorStruct* error = get_error();
	CHECK_IF_NULL(error);
	*message = error->msg;
	return error->err;
}

void error_set(UserErrorType err_type, Error error, const char* arg)
{
	struct ErrorStruct* error_s = get_error();
	CHECK_IF_NULL(error_s);

	const char* instr_err_str = "Instruction file compilation error:";
	const char* prog_err_str = "Program file compilation error:";
	const char* runtime_err_str = "Runtime error:";
	const char* comp_format_array[] = {
		"%s cannot access \"%s\" file\n",
		"%s cannot find \"%s\"\n",
		"%s invalid value \"%s\"\n"
	};
	const char* runtime_format_array[] = {
		"%s already stopped\n"
	};
	const char empty[] = "";
	if (!arg)
		arg = empty;

	const char* header = NULL;
	const char* format = NULL;

	switch (err_type)
	{
	case INSTR_COMP_ERROR:
		header = instr_err_str;
		break;
	case PROG_COMP_ERROR:
		header = instr_err_str;
		break;
	case RUNTIME_ERROR:
	{
		header = instr_err_str;
		switch (error)
		{
		case CPU_STOPPED:
			format = runtime_format_array[0];
			break;
		default:
			CRASH_LOG(LOG_UNKNOWN_VALUE);
			break;
		}
	}
	break;
	default:
		CRASH_LOG(LOG_UNKNOWN_VALUE);
		break;
	}

	if (err_type == INSTR_COMP_ERROR || err_type == PROG_COMP_ERROR)
		switch (error)
		{
		case NO_FILE:
			format = comp_format_array[0];
			break;
		case MISSING_INPUT:
			format = comp_format_array[1];
			break;
		case INVALID_INPUT:
			format = comp_format_array[2];
			break;
		default:
			CRASH_LOG(LOG_UNKNOWN_VALUE);
			break;
		}

	size_t new_msg_len = strlen(format) + strlen(header) + strlen(arg) + 1;
	char* new_msg = malloc_s(new_msg_len);
	if (sprintf_s(new_msg, new_msg_len, format, header, arg) <= 0)
		CRASH_LOG(LIBRARY_FUNC_FAILURE);
	if (error_s->msg)
		free((char*)error_s->msg);
	*error_s = (struct ErrorStruct){ err_type, new_msg };
}

inline void instr_error_set(CompilationError error, const char* arg)
{
	error_set(INSTR_COMP_ERROR, error, arg);
}

inline void prog_error_set(CompilationError error, const char* arg)
{
	error_set(PROG_COMP_ERROR, error, arg);
}

inline void runtime_error_set(RuntimeError error, const char* arg)
{
	error_set(RUNTIME_ERROR, error, arg);
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
