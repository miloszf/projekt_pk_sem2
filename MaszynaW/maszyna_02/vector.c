#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "error.h"

struct Vector
{
	size_t el_size;
	size_t el_number;
	struct Node
	{
		void* value_ptr;
		struct Node* next;
		struct Node* prev;
	} *head, *tail;
};

struct Vector* vector_init(size_t element_size)
{
	struct Vector* new_vect = malloc_s(sizeof(struct Vector));
	*new_vect = (struct Vector){ element_size, 0, NULL, NULL };
	return new_vect;
}

void* vector_push(struct Vector* vect, void* value_ptr)
{
	CHECK_IF_NULL(vect);
	CHECK_IF_NULL(value_ptr);

	void* new_value_ptr = malloc(vect->el_size);
	memcpy(new_value_ptr, value_ptr, vect->el_size);
	if (vect->head)
	{
		vect->tail->next = malloc_s(sizeof(struct Node));
		*vect->tail->next = (struct Node){ new_value_ptr, NULL, vect->tail };
		vect->tail = vect->tail->next;
	}
	else
	{
		vect->head = malloc_s(sizeof(struct Node));
		*vect->head = (struct Node){ new_value_ptr, NULL, NULL };
		vect->tail = vect->head;
	}
	vect->el_number++;
	return vect->tail->value_ptr;
}

void* vector_pop(struct Vector* vect)
{
	CHECK_IF_NULL(vect);
	void* return_ptr;
	if (vect->el_number)
	{
		return_ptr = vect->tail->value_ptr;
		vect->el_number--;
		if (vect->el_number)
		{
			vect->tail = vect->tail->prev;
			free(vect->tail->next);
			vect->tail->next = NULL;
		}
		else
		{
			free(vect->tail);
			vect->tail = NULL;
			vect->head = NULL;
		}
	}
	else
		return_ptr =  NULL;
	return return_ptr;
}

struct Node* vector_find(struct Vector* vect, size_t index)
{
	CHECK_IF_NULL(vect);
	if (index < vect->el_number)
	{
		struct Node* node = vect->head;
		while (index--)
			node = node->next;
		return node;
	}
	else
		return NULL;
}

void* vector_write(struct Vector* vect, size_t index, void* value_ptr)
{
	struct Node* node = vector_find(vect, index);
	if (node)
	{
		memcpy(node->value_ptr, value_ptr, vect->el_size);
		return node->value_ptr;
	}
	else
		return NULL;
}

void* vector_read(struct Vector* vect, size_t index)
{
	struct Node* node = vector_find(vect, index);
	if (node)
		return node->value_ptr;
	else
		return NULL;
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
	void* new_array = malloc_s(vect->el_size * vect->el_number);
	struct Node* iter = vect->head;
	for (unsigned i = 0; iter; i += vect->el_size, iter = iter->next)
		memcpy((char*)new_array + i, iter->value_ptr, vect->el_size);
	vector_delete(vect);
	return new_array;
}

struct Vector* vector_copy(struct Vector* vect)
{
	CHECK_IF_NULL(vect);
	struct Vector* new_vect = vector_init(vect->el_size);
	for (struct Node* iter = vect->head; iter; iter = iter->next)
		vector_push(new_vect, iter->value_ptr);
	return new_vect;
}

void vector_delete(struct Vector* vect)
{
	if (vect)
	{
		while (vect->head)
		{
			struct Node* iter = vect->head->next;
			free(vect->head->value_ptr);
			free(vect->head);
			vect->head = iter;
		}
		free(vect);
	}
}