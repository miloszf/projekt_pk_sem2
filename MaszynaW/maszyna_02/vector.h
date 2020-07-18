#ifndef VECTOR_H
#define VECTOR_H

/** Struktura s³u¿¹ca do przechowywania danych, zrealizowana jako lista dwukierunkowa.
	Umo¿liwia dopisywanie i usuwanie elementów z koñca listy oraz odczytywanie i nadpisywanie 
	dowolnie wybranych elementów. */
struct Vector;

/** Funkcja inicjalizuj¹ca obiekt struktury Vector.
@param element_size rozmiar elementu do zapisania
@return nowy obiekt */
struct Vector* vector_init(size_t element_size);
/** Funkcja dopisuj¹ca element do koñca wektora. Podany element zostaje skopiowany.
@param vect wektor, do którego zostanie dopisany element
@param value_ptr wskaŸnik na element do zapisania 
@return wskaŸnik na miejsce do którego zosta³ przekopiowany podany element */
void* vector_push(struct Vector* vect, void* value_ptr);
/** Funkcja usuwaj¹ca element z koñca wektora.
	Usuwany jest tylko ostatni wêze³ listy, natomiast zapisana w nim wartoœæ - do której
	zwracany jest wskaŸnik - pozostaje nienaruszona.
@param vect wektor, z którego zostanie usuniêty element
@return wskaŸnik na miejsce w którym znajduje siê wartoœæ ostatniego elementu listy */
void* vector_pop(struct Vector* vect);
/** Funkcja zapisuj¹ca element pod podanym indeksem.
@param vect wektor, którego element zostanie nadpisany
@param index indeks do zapisania
@param value_ptr wskaŸnik na element do zapisania
@return wskaŸnik na miejsce w którym znajduje nadpisany element */
void* vector_write(struct Vector* vect, size_t index, void* value_ptr);
/** Funkcja zwracaj¹ca element z pod wybranego indeksu.
@param vect wektor, którego element zostanie odczytany
@param index indeks do odczytania
@return wskaŸnik na odczytany element */
void* vector_read(struct Vector* vect, size_t index);
/** Funkcja zwracaj¹ca liczbê zapisanych elementów.
@param vect wektor
@return liczba zapisanych elementów */
size_t vector_size(struct Vector* vect);
/** Funkcja konwertuj¹ca wektor na tablicê.
@param vect wektor do konwersji
@param size wskaŸnik na zmienn¹, do której zostanie zapisany rozmiar tablicy
@return nowa tablica obiektów */
void* vector_convert_to_array(struct Vector* vect, size_t* size);
/** Funkcja zwaracaj¹ca kopiê podanego wektora.
@param vect wektor do skopiwania
@return nowy wektor */
struct Vector* vector_copy(struct Vector* vect);
/** Funkcja usuwaj¹ca wektor. Usuwane s¹ wszystkie wêz³y wraz z zawartoœci¹.
	Aby móc dalej korzystaæ z wektora trzeba go ponownie zainicjalizowaæ.
	Korzystanie z usuniêtego wektora doprowadzi do naruszenia pamiêci.
@param vect wektor do usuniêcia */
void vector_delete(struct Vector* vect);

#endif