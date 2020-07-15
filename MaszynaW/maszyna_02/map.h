#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

/** Struktura przechowuj�ca par� wektor�w - wektor kluczy i wektor odpowiadaj�cych im warto�ci.
	Umo�liwia dopisywanie element�w do ko�ca listy oraz odczytywanie elementu z podaneg klucza.
	W mapie mog� znajdowa� si� tylko unikalne klucze. Kluczem jest �a�cuch znak�w, warto�ci� - dowolny obiekt */
struct Map;

/** Funkcja inicjalizuj�ca obiekt struktury Map.
@param element_size rozmiar elementu do zapisania
@return wska�nik na nowy obiekt */
struct Map* map_init(size_t element_size);
/** Funkcja dopisuj�ca element do mapy. Zwraca 'true' je�li element uda�o si� dopisa�,
	'false' gdy podany klucz ju� wyst�puje w mapie.
@param map wska�nik na map�, do kt�rej zostanie dopisany element
@param key klucz, �a�cuch znak�w zako�czony zerem
@param value_ptr wska�nik na element do zapisania
@return powodzenie zapisu */
bool map_push(struct Map* map, const char* key, void* value_ptr);
/** Funkcja odczytuj�ca element z mapy. Je�li element uda�o si� znale�� zwraca element 
	odpowiadaj�cy danemu kluczowi lub 'NULL' w przeciwnym przypadku
@param map wska�nik na map�, z kt�rej zostanie odczytany element
@param key klucz, �a�cuch znak�w zako�czony zerem
@return wska�nik na odczytany element */
void* map_read_from_key(struct Map* map, const char* key);
/** Funkcja usuwaj�ca map�.
	Aby m�c ponownie korzysta� z mapy trzeba j� ponownie zainicjalizowa�.
	Korzystanie ze skasowanej mapy doprowadzi do naruszenia pami�ci.
@param map wska�nik na map� do usuni�cia */
void map_delete(struct Map* map);

#endif