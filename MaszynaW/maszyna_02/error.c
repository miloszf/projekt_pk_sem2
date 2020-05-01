#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "error.h"

#include <stdbool.h>
bool debug_deleted = false;

#define EXIT_VALUE -1



struct StructError {
	Error err;
	char* msg;
};

void error_exit(void);

struct StructError* get_error()
{
	static struct StructError* s_error = NULL;
	if (s_error)
		return s_error;
	else
	{
		s_error = malloc(sizeof(struct StructError));
		if (!s_error || atexit(&error_exit))
			exit(EXIT_VALUE);
		else
			*s_error = (struct StructError){ 0, NULL };
		// DEBUG
		return s_error;
	}
}

Error error_get()
{
	return get_error()->err;
}

void error_set(Error error)
{
	get_error()->err = error;
}

void error_set_msg(Error error, const char* message)
{
	struct StructError* s_err = get_error();
	if (s_err->msg)
		free(s_err->msg);
	s_err->msg = _strdup(message);
	if (!s_err->msg)
		exit(EXIT_VALUE);
	s_err->err = error;
}

void critical_error_set(const char* message)
{
	struct StructError* s_err = get_error();
	if (s_err->msg)
		free(s_err->msg);
	s_err->msg = _strdup(message);
	exit(EXIT_VALUE);
}

const char* error_get_msg()
{
	return get_error()->msg;
}

void* malloc_s(size_t size)
{
	void* value_ptr = malloc(size);
	if (!value_ptr)
		critical_error_set("B³¹d alokacji pamiêci w funkcji malloc.");
	return value_ptr;
}

void* calloc_s(size_t count, size_t size)
{
	void* value_ptr = calloc(count, size);
	if (!value_ptr)
		critical_error_set("B³¹d alokacji pamiêci w funkcji calloc.");
	return value_ptr;
}

void* realloc_s(void* ptr, size_t new_size)
{
	void* value_ptr = realloc(ptr, new_size);
	if (!value_ptr)
		critical_error_set("B³¹d alokacji pamiêci w funkcji realloc.");
	return value_ptr;
}

void check_for_NULL(const void* ptr)
{
	if (!ptr)
		critical_error_set("Wy³uskanie wskaŸnika NULL.");
}

void error_exit(void)
{
	//DEBUG
	if (!debug_deleted)
	{
		struct StructError* s_err = get_error();
		if (s_err->err)
			if (s_err->msg)
				printf("%s\n", s_err->msg);
			else
				printf("Wyst¹pi³ b³¹d!\n");
		free(s_err->msg);
		free(s_err);
	}

}

void debug_error_delete()
{
	debug_deleted = true;
	struct StructError* s_err = get_error();
	free(s_err);
}