#ifndef MAP_H
#define MAP_H

// Struktura przechowuj�ca par� wektor�w - klucz i warto��.
struct Map;

struct Map* map_init(size_t element_size);
void map_push(struct Map* map, const char* key, void* value_ptr);
void* map_read_from_key(struct Map* map, const char* key);
void map_delete(struct Map* map);

#endif