#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "vector.h"
#include "error.h"

#define CHUNK_SIZE 4 

struct Map
{
	//size_t el_size;
	//size_t el_number;
	//size_t array_size;
	struct Vector* keys_vect;
	struct Vector* values_vect;
};

struct Map* map_init(size_t element_size)
{
	struct Map* new_map = malloc_s(sizeof(struct Map));
	new_map->keys_vect = vector_init(sizeof(char*));
	new_map->values_vect = vector_init(element_size);
	return new_map;
}

bool map_push(struct Map* map, const char* key, void* value_ptr)
{
	//check_for_NULL(map);
	//check_for_NULL(key);
	check_for_NULL(value_ptr);

	if (!map_read_from_key(map, key))
	{
		char* new_key = _strdup(key);
		check_for_NULL(new_key);
		vector_push(map->keys_vect, &new_key);
		vector_push(map->values_vect, value_ptr);
		return true;
	}
	else
		return false;
}

void* map_read_from_key(struct Map* map, const char* key)
{
	check_for_NULL(map);
	check_for_NULL(key);
	void* value = NULL;
	size_t map_size = vector_size(map->keys_vect);
	for (size_t i = 0; i < map_size; i++)
	{
		const char** key_ptr = vector_read(map->keys_vect, i);
		check_for_NULL(key_ptr);
		if (!strcmp(key, *key_ptr))
		{
			value = vector_read(map->values_vect, i);
			break;
		}
	}

	return value;
}

void map_delete(struct Map* map)
{
	if (map)
	{
		char** key_ptr;
		while (key_ptr = vector_pop(map->keys_vect))
			free(*key_ptr);
		vector_delete(map->keys_vect);
		vector_delete(map->values_vect);
		free(map);
	}
}