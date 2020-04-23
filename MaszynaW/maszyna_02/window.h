#ifndef WINDOW_H
#define WINDOW_H

#include "settings.h"
#include "vector.h"

// Struktura zajmuj¹ca siê wyœwietlaniem grafiki, przechowuje informacje o
// rozmiarze okna i zapisanych p³ótnach. Rysuje obiekty przypisane do poszczególnych p³ócien.
struct Window
{
	struct Pixel* buffer;
	Point size;
	struct Vector* canvas_vect;
};

// Funkcja inicjalizuj¹ca okno, zwraca wskaŸnik na nowy obiekt.
struct Window* window_init();

// Funkcja zwracaj¹ca wskaŸnik nowe p³ótno, które zostaje dopisane do wektora p³ócien okna. 
// Dopasowuje rozmiar wewnêtrznego bufora w zale¿noœci od rozmiaru p³ótna. 
// argumenty: rozmiar i po³o¿enie p³ótna
struct Canvas* window_new_canvas(struct Window* window, Point offset, Point size);

// Funkcja rysuj¹ca zapisane plótna w wewnêtrzym buforze
// argumenty: wskaŸnik na okno
void window_draw(struct Window* window);

// Funkcja zwracaj¹ca wewnêtrzny bufor okna i jego wymiar
// argumenty: wskaŸnik na okno, adres wskaŸnika do przypisania do bufora, 
// adres struktury do zapisania rozmiaru okna
//void window_get_buffer(struct Window* window, void** buffer, Point* buffer_size);

// Funkcja zamieniaj¹ca liniê w podanym pikselu na znak unicode
void window_line_to_wchar(struct Pixel* pixel);

void window_clear_buffer(struct Window* window);

void window_delete(struct Window* window);

#endif


