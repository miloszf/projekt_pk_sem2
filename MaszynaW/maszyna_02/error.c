#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include "error.h"

#define EXIT_VALUE -1

struct StructError {
	Error err;
	const char* msg;
};

struct StructError* get_error()
{
	static struct StructError* s_error = NULL;
	if (s_error)
		return s_error;
	else
	{
		s_error = malloc(sizeof(struct StructError));
		if (!s_error)
			assert(!"B³¹d obs³ugi b³êdów!");
		else
			*s_error = (struct StructError){ 0, NULL };

		return s_error;
	}
}

Error error()
{
	return get_error()->err;
}

void set_error(Error error, const char* message)
{
	struct StructError* s_err = get_error();
	s_err->err = error;
	s_err->msg = message;
}

void* malloc_s(size_t size)
{
	//struct StructError* s_err = get_error();
	void* value_ptr = malloc(size);
	if (!value_ptr)
		exit(EXIT_VALUE);
	return value_ptr;
}

void* calloc_s(size_t count, size_t size)
{
	//struct StructError* s_err = get_error();
	void* value_ptr = calloc(count, size);
	if (!value_ptr)
		exit(EXIT_VALUE);
	return value_ptr;
}

void* realloc_s(void* ptr, size_t new_size)
{
	//struct StructError* s_err = get_error();
	void* value_ptr = realloc(ptr, new_size);
	if (!value_ptr)
		exit(EXIT_VALUE);
	return value_ptr;
}

void check_for_NULL(void* ptr)
{
	if (!ptr)
		exit(EXIT_VALUE);
}