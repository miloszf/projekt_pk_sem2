#ifndef VECTOR_H
#define VECTOR_H

// Struktura przechowuj�ca tablic� o zmiennym rozmiarze.
// Automatyczne rozszerza si� w miar� potrzeby. Przechowuje
// wska�nik na warto�ci, rozmiar tablicy i zapisanego elementu,
// liczb� zapisanych element�w. Tablica widoczna, reszta schowana?

struct Vector;

// Funkcja inicjalizuj�ca struktur�
// argumenty: rozmiar elementu do zapisania
struct Vector* vector_init(size_t element_size);

// Funkcja dopisuj�ca element do ko�ca wektora
void* vector_push(struct Vector* vect, void* value_ptr);
// Funkcja usuwaj�ca i zwracaj�ca element z ko�ca wektora
void* vector_pop(struct Vector* vect);
// Funkcja zapisuj�ca element pod podanym indeksem
void* vector_write(struct Vector* vect, size_t index, void* value_ptr);
// Funkcja zwracaj�ca element z pod wybranego indeksu
void* vector_read(struct Vector* vect, size_t index);

// Funkcja zwracaj�ca liczb� zapisanych element�w
size_t vector_size(struct Vector* vect);
// Funkcja usuwaj�ca wektor, zwraca tablic� element�w i ich liczb�
void* vector_unwrap(struct Vector* vect, size_t* size);

// Funkcja czyszcz�ca wektor z element�w i zapisanych ustawie�.
void vector_delete(struct Vector* vect);

#endif