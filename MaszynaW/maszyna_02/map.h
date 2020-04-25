#ifndef MAP_H
#define MAP_H

// Struktura przechowuj¹ca parê wektorów - klucz i wartoœæ.
struct Map;

struct Map* map_init(size_t element_size);
void map_push(struct Map* map, const char* key, void* value_ptr);
void* map_read_from_key(struct Map* map, const char* key);
void map_delete(struct Map* map);

#endif