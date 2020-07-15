#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

// Struktura przechowuj�ca par� wektor�w - wektor kluczy i wektor odpowiadaj�cych im warto�ci.
struct Map;

struct Map* map_init(size_t element_size);
bool map_push(struct Map* map, const char* key, void* value_ptr);
void* map_read_from_key(struct Map* map, const char* key);
void map_delete(struct Map* map);

#endif