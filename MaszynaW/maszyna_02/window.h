#ifndef WINDOW_H
#define WINDOW_H

#include "settings.h"
#include "vector.h"

/** Struktura reprezentuj�ca okno, w kt�rym zawarte s� sceny. Rozmiar okna dopasowuje si� automatycznie
	do zawartych w nim scen. */
struct Window
{
	struct Pixel* buffer;		///< bufor graficzny okna
	Point size;					///< rozmiar okna
	struct Vector* canvas_vect;	///< wektor scen
};

/** Funkcja inicjalizuj�ca obiekt struktury Window.
@return nowy obiekt */
struct Window* window_init();
/** Funkcja inicjalizuj�ca obiekt struktury Canvas przypisany do danego okna.
@param window okno
@param offset przesuni�cie wzgl�dem lewego g�rnego rogu okna
@param size rozmiar sceny
@return nowy obiekt */
struct Canvas* window_new_canvas(struct Window* window, Point offset, Point size);
/** Funkcja renderuj�ca dane okno.
@param window okno */
void window_draw(struct Window* window);
/** Funkcja zamieniaj�ca lini� w podanym pikselu na znak unicode.
@param pixel piksel do konwersji */
void window_line_to_wchar(struct Pixel* pixel);
/** Funkcja czyszcz�ca bufor wybranego okna.
@param window okno */
void window_clear_buffer(struct Window* window);
/** Funkcja usuwaj�ca podany obiekt.
@param window obiekt do usuni�cia */
void window_delete(struct Window* window);

#endif


