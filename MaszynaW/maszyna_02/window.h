#ifndef WINDOW_H
#define WINDOW_H

#include "settings.h"
#include "vector.h"

// Struktura zajmuj�ca si� wy�wietlaniem grafiki, przechowuje informacje o
// rozmiarze okna i zapisanych p��tnach. Rysuje obiekty przypisane do poszczeg�lnych p��cien.
struct Window
{
	struct Pixel* buffer;
	Point size;
	struct Vector* canvas_vect;
};

// Funkcja inicjalizuj�ca okno, zwraca wska�nik na nowy obiekt.
struct Window* window_init();

// Funkcja zwracaj�ca wska�nik nowe p��tno, kt�re zostaje dopisane do wektora p��cien okna. 
// Dopasowuje rozmiar wewn�trznego bufora w zale�no�ci od rozmiaru p��tna. 
// argumenty: rozmiar i po�o�enie p��tna
struct Canvas* window_new_canvas(struct Window* window, Point offset, Point size);

// Funkcja rysuj�ca zapisane pl�tna w wewn�trzym buforze
// argumenty: wska�nik na okno
void window_draw(struct Window* window);

// Funkcja zwracaj�ca wewn�trzny bufor okna i jego wymiar
// argumenty: wska�nik na okno, adres wska�nika do przypisania do bufora, 
// adres struktury do zapisania rozmiaru okna
//void window_get_buffer(struct Window* window, void** buffer, Point* buffer_size);

// Funkcja zamieniaj�ca lini� w podanym pikselu na znak unicode
void window_line_to_wchar(struct Pixel* pixel);

void window_clear_buffer(struct Window* window);

void window_delete(struct Window* window);

#endif


