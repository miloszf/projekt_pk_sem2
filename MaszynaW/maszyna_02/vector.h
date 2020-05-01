#ifndef VECTOR_H
#define VECTOR_H

// Struktura przechowuj¹ca tablicê o zmiennym rozmiarze.
// Automatyczne rozszerza siê w miarê potrzeby. Przechowuje
// wskaŸnik na wartoœci, rozmiar tablicy i zapisanego elementu,
// liczbê zapisanych elementów. Tablica widoczna, reszta schowana?

struct Vector;

// Funkcja inicjalizuj¹ca strukturê
// argumenty: rozmiar elementu do zapisania
struct Vector* vector_init(size_t element_size);

// Funkcja dopisuj¹ca element do koñca wektora
void* vector_push(struct Vector* vect, void* value_ptr);
// Funkcja usuwaj¹ca i zwracaj¹ca element z koñca wektora
void* vector_pop(struct Vector* vect);
// Funkcja zapisuj¹ca element pod podanym indeksem
void* vector_write(struct Vector* vect, size_t index, void* value_ptr);
// Funkcja zwracaj¹ca element z pod wybranego indeksu
void* vector_read(struct Vector* vect, size_t index);

// Funkcja zwracaj¹ca liczbê zapisanych elementów
size_t vector_size(struct Vector* vect);
// Funkcja usuwaj¹ca wektor, zwraca tablicê elementów i ich liczbê
void* vector_unwrap(struct Vector* vect, size_t* size);

// Funkcja czyszcz¹ca wektor z elementów i zapisanych ustawieñ.
void vector_delete(struct Vector* vect);

#endif