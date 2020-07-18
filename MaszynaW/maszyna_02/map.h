#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

/** Struktura przechowuj¹ca parê wektorów - wektor kluczy i wektor odpowiadaj¹cych im wartoœci.
	Umo¿liwia dopisywanie elementów do koñca mapy oraz odczytywanie elementu z podaneg klucza.
	W mapie mog¹ znajdowaæ siê tylko unikalne klucze. Kluczem jest ³añcuch znaków, wartoœci¹ - dowolny obiekt. */
struct Map;

/** Funkcja inicjalizuj¹ca obiekt struktury Map.
@param element_size rozmiar elementu do zapisania
@return wskaŸnik na nowy obiekt */
struct Map* map_init(size_t element_size);
/** Funkcja dopisuj¹ca element do mapy. Zwraca 'true' jeœli element uda³o siê dopisaæ,
	'false' gdy podany klucz ju¿ wystêpuje w mapie.
@param map mapa do której zostanie dopisany element
@param key klucz, ³añcuch znaków zakoñczony zerem
@param value_ptr wskaŸnik na element do zapisania
@return powodzenie zapisu */
bool map_push(struct Map* map, const char* key, void* value_ptr);
/** Funkcja odczytuj¹ca element z mapy. Jeœli element uda³o siê znaleœæ zwraca element 
	odpowiadaj¹cy danemu kluczowi, 'NULL' w przeciwnym przypadku.
@param map mapa z której zostanie odczytany element
@param key klucz, ³añcuch znaków zakoñczony zerem
@return wskaŸnik na odczytany element lub 'NULL' */
void* map_read_from_key(struct Map* map, const char* key);
/** Funkcja usuwaj¹ca mapê.
	Aby móc dalej korzystaæ z mapy trzeba j¹ ponownie zainicjalizowaæ.
	Korzystanie ze skasowanej mapy doprowadzi do naruszenia pamiêci.
@param map mapa do usuniêcia */
void map_delete(struct Map* map);

#endif