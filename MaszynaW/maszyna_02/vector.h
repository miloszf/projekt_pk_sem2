#ifndef VECTOR_H
#define VECTOR_H

/** Struktura s�u��ca do przechowywania daych, zrealizowana jako lista dwukierunkowa.
	Umo�liwia dopisywanie i usuwanie element�w z ko�ca listy oraz odczytywanie i nadpisywanie 
	dowolnie wybranych element�w. */
struct Vector;

/** Funkcja inicjalizuj�ca obiekt struktury Vector.
@param element_size rozmiar elementu do zapisania
@return wska�nik na nowy obiekt */
struct Vector* vector_init(size_t element_size);
/** Funkcja dopisuj�ca element do ko�ca wektora. Podany element zostaje skopiowany.
@param vect wska�nik na wektor, do kt�rego zostanie dopisany element
@param value_ptr wska�nik na element do zapisania 
@return wska�nik na miejsce do kt�rego zosta� przekopiowany podany element */
void* vector_push(struct Vector* vect, void* value_ptr);
/** Funkcja usuwaj�ca element z ko�ca wektora.
	Usuwany jest tylko ostatni w�ze� listy, natomiast zapisana w nim warto�� - do kt�rej
	zwracany jest wska�nik - pozostaje nienaruszona.
@param vect wska�nik na wektor, z kt�rego zostanie usuni�ty element
@return wska�nik na miejsce w kt�rym znajduje si� ostatni element listy */
void* vector_pop(struct Vector* vect);
/** Funkcja zapisuj�ca element pod podanym indeksem.
@param vect wska�nik na wektor, kt�rego element zostanie nadpisany
@param index indeks do zapisania
@param value_ptr wska�nik na element do zapisania
@return wska�nik na miejsce w kt�rym znajduje nadpisany element */
void* vector_write(struct Vector* vect, size_t index, void* value_ptr);
/** Funkcja zwracaj�ca element z pod wybranego indeksu.
@param vect wska�nik na wektor, kt�rego element zostanie odczytany
@param index indeks do odczytania
@return wska�nik odczytany element */
void* vector_read(struct Vector* vect, size_t index);
/** Funkcja zwracaj�ca liczb� zapisanych element�w.
@param vect wska�nik na wektor
@return liczba zapisanych element�w */
size_t vector_size(struct Vector* vect);
/** Funkcja konwertuj�ca wektor na tablic�.
@param vect wska�nik na wektor do konwersji
@param size wska�nik na zmienn�, do kt�rej zostanie zapisany rozmiar tablicy
@return liczba zapisanych element�w */
void* vector_convert_to_array(struct Vector* vect, size_t* size);
/** Funkcja zwaracaj�ca kopi� podanego wektora.
@param vect wska�nik na wektor do skopiwania
@return wska�nik na nowy wektor */
struct Vector* vector_copy(struct Vector* vect);
/** Funkcja usuwaj�ca wektor. Usuwane s� wszystkie w�z�y wraz z zawarto�ci�.
	Aby m�c ponownie korzysta� z wektora trzeba go ponownie zainicjalizowa�.
	Korzystanie z usuni�tego wektora doprowadzi do naruszenia pami�ci.
@param vect wska�nik na wektor do usuni�cia */
void vector_delete(struct Vector* vect);

#endif