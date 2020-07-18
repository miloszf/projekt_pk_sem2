#ifndef CANVAS_H
#define CANVAS_H

#include "settings.h"

/** Struktura reprezentuj¹ca scenê w której zawarte s¹ graficzne reprezentacje obiektów. */
struct Canvas
{
	Point position;					///< pozycja wzglêdem lewego górnego rogu okna 
	Point size;						///< rozmiar sceny
	struct Vector* drawable_vect;	///< wektor graficznych reprezentacji obiektów zawartych w scenie
};

/** Funkcja inicjalizuj¹ca obiekt struktury Canvas.
@param position pozycja wzglêdem lewego górnego rogu okna
@param size rozmiar sceny
@return nowy obiekt */
struct Canvas* canvas_init(Point position, Point size);
/** Funkcja inicjalizuj¹ca obiekt struktury Drawable przypisany do danej sceny
@param canvas scena
@param position pozycja wzglêdem lewego górnego rogu sceny
@return nowy obiekt */
struct Drawable* canvas_new_drawable(struct Canvas* canvas, Point position);
/** Funkcja renderuj¹ca dan¹ scenê.
@param canvas scena
@param r_info obiekt przechowuj¹cy informacje potrzebne do wygenerowania grafiki */
void canvas_draw(struct Canvas* canvas, struct RenderInfo r_info);
/** Funkcja usuwaj¹ca podany obiekt.
@param canvas obiekt do usuniêcia */
void canvas_delete(struct Canvas* canvas);

#endif