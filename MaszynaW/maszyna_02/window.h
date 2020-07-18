#ifndef WINDOW_H
#define WINDOW_H

#include "settings.h"
#include "vector.h"

/** Struktura reprezentuj¹ca okno, w którym zawarte s¹ sceny. Rozmiar okna dopasowuje siê automatycznie
	do zawartych w nim scen. */
struct Window
{
	struct Pixel* buffer;		///< bufor graficzny okna
	Point size;					///< rozmiar okna
	struct Vector* canvas_vect;	///< wektor scen
};

/** Funkcja inicjalizuj¹ca obiekt struktury Window.
@return nowy obiekt */
struct Window* window_init();
/** Funkcja inicjalizuj¹ca obiekt struktury Canvas przypisany do danego okna.
@param window okno
@param offset przesuniêcie wzglêdem lewego górnego rogu okna
@param size rozmiar sceny
@return nowy obiekt */
struct Canvas* window_new_canvas(struct Window* window, Point offset, Point size);
/** Funkcja renderuj¹ca dane okno.
@param window okno */
void window_draw(struct Window* window);
/** Funkcja zamieniaj¹ca liniê w podanym pikselu na znak unicode.
@param pixel piksel do konwersji */
void window_line_to_wchar(struct Pixel* pixel);
/** Funkcja czyszcz¹ca bufor wybranego okna.
@param window okno */
void window_clear_buffer(struct Window* window);
/** Funkcja usuwaj¹ca podany obiekt.
@param window obiekt do usuniêcia */
void window_delete(struct Window* window);

#endif


