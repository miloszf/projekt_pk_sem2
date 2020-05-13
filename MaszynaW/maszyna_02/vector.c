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
	void** ptr_array;
};

struct Vector* vector_init(size_t element_size)
{
	struct Vector* new_vect = malloc_s(sizeof(struct Vector));
	*new_vect = (struct Vector){ element_size, 0, CHUNK_SIZE };
	new_vect->ptr_array = malloc_s(sizeof(void*));
	*new_vect->ptr_array = malloc_s(CHUNK_SIZE * new_vect->el_size);
	return new_vect;
}

void* vector_push(struct Vector* vect, void* value_ptr)
{
	CHECK_IF_NULL(vect);
	CHECK_IF_NULL(value_ptr);

	if (vect->el_number >= vect->array_size)
	{
		vect->array_size += CHUNK_SIZE;
		size_t ptr_array_new_size = vect->array_size / CHUNK_SIZE;
		vect->ptr_array = realloc_s(vect->ptr_array, sizeof(void*) * ptr_array_new_size);
		vect->ptr_array[ptr_array_new_size - 1] = malloc_s(CHUNK_SIZE * vect->el_size);
	}

	void* dest = (char*)(vect->ptr_array[vect->el_number / CHUNK_SIZE]) + (vect->el_size * (vect->el_number % CHUNK_SIZE));
	memcpy(dest, value_ptr, vect->el_size);
	vect->el_number++;
	return dest;
}

void* vector_pop(struct Vector* vect)
{
	CHECK_IF_NULL(vect);
	if (vect->el_number)
	{
		vect->el_number--;
		//return (char*)vect->array + (vect->el_size * vect->el_number);
		return (char*)(vect->ptr_array[vect->el_number / CHUNK_SIZE]) + (vect->el_size * (vect->el_number % CHUNK_SIZE));
	}
	else
		return NULL;
}

void* vector_write(struct Vector* vect, size_t index, void* value_ptr)
{
	CHECK_IF_NULL(vect);
	if (index < vect->el_number)
	{
		//void* dest = (char*)vect->array + (vect->el_size * index);
		void* dest = (char*)(vect->ptr_array[index / CHUNK_SIZE]) + (vect->el_size * (index % CHUNK_SIZE));
		memcpy(dest, value_ptr, vect->el_size);
		return dest;
	}
	else
		return NULL;
}

void* vector_read(struct Vector* vect, size_t index)
{
	CHECK_IF_NULL(vect);
	return (index < vect->el_number) ? (char*)(vect->ptr_array[index / CHUNK_SIZE]) + (vect->el_size * (index % CHUNK_SIZE)) : NULL;
}

size_t vector_size(struct Vector* vect)
{
	CHECK_IF_NULL(vect);
	return vect->el_number;
}

void* vector_convert_to_array(struct Vector* vect, size_t* size)
{
	CHECK_IF_NULL(vect);
	if (size)
		*size = vect->el_number;
	void* new_array = malloc_s(vect->el_size * vect->array_size);
	for (unsigned i = 0; i < (vect->array_size / CHUNK_SIZE); i++)
	{
		memcpy((char*)new_array + (i * CHUNK_SIZE * vect->el_size), vect->ptr_array[i], (vect->el_size * CHUNK_SIZE));
	}
	vector_delete(vect);
	return new_array;
}

struct Vector* vector_copy(struct Vector* vect)
{
	CHECK_IF_NULL(vect);
	struct Vector* new_vect = malloc_s(sizeof(struct Vector));
	*new_vect = (struct Vector){ vect->el_size, vect->el_number, vect->array_size };
	new_vect->ptr_array = malloc_s(sizeof(void*) * vect->array_size / CHUNK_SIZE);
	for (unsigned i = 0; i < (vect->array_size / CHUNK_SIZE); i++)
	{
		new_vect->ptr_array[i] = malloc_s(vect->el_size * CHUNK_SIZE);
		memcpy(new_vect->ptr_array[i], vect->ptr_array[i], vect->el_size * CHUNK_SIZE);
	}
	return new_vect;
}

void vector_delete(struct Vector* vect)
{
	if (vect)
	{
		for (unsigned i = 0; i < (vect->array_size / CHUNK_SIZE); i++)
			free(vect->ptr_array[i]);
		free(vect->ptr_array);
		free(vect);
	}
}