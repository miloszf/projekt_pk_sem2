#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "error.h"

#define CHUNK_SIZE 4 

struct Vector
{
	size_t el_size;
	size_t el_number;
	size_t array_size;
	void* array;
};

struct Vector* vector_init(size_t element_size)
{
	struct Vector* vect_ptr = malloc_s(sizeof(struct Vector));
	*vect_ptr = (struct Vector){ element_size, 0, CHUNK_SIZE };
	vect_ptr->array = malloc_s(CHUNK_SIZE * vect_ptr->el_size);
	return vect_ptr;
}

void* vector_push(struct Vector* vect, void* value_ptr)
{
	check_for_NULL(vect);
	check_for_NULL(value_ptr);

	if (vect->el_number >= vect->array_size)
	{
		vect->array_size += CHUNK_SIZE;
		vect->array = realloc_s(vect->array, vect->array_size * vect->el_size);
	}

	void* dest = (char*)vect->array + (vect->el_size * vect->el_number);
	memcpy(dest, value_ptr, vect->el_size);
	vect->el_number++;
	return dest;
}

//void* vector_pop(struct Vector* vect)
//{
//	check_for_NULL(vect);
//	if (vect->el_number)
//	{
//		vect->el_number--;
//		void* return_ptr = malloc_s(vect->el_size);
//		void* value_ptr = (char*)vect->array + (vect->el_size * vect->el_number);
//		memcpy(return_ptr, value_ptr, vect->el_size);
//		return return_ptr;
//	}
//	else
//		return NULL;
//}

void* vector_pop(struct Vector* vect)
{
	check_for_NULL(vect);
	if (vect->el_number)
	{
		vect->el_number--;
		return (char*)vect->array + (vect->el_size * vect->el_number);
	}
	else
		return NULL;
}

void* vector_write(struct Vector* vect, size_t index, void* value_ptr)
{
	check_for_NULL(vect);
	if (index < vect->el_number)
	{
		void* dest = (char*)vect->array + (vect->el_size * index);
		memcpy(dest, value_ptr, vect->el_size);
		return dest;
	}
	else
		return NULL;
}

void* vector_read(struct Vector* vect, size_t index)
{
	check_for_NULL(vect);
	return (index < vect->el_number) ? (char*)vect->array + (vect->el_size * index) : NULL;
}

size_t vector_size(struct Vector* vect)
{
	check_for_NULL(vect);
	return vect->el_number;
}

void vector_delete(struct Vector* vect)
{
	if (vect)
	{
		free(vect->array);
		free(vect);
	}
}