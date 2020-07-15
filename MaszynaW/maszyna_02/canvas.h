#ifndef CANVAS_H
#define CANVAS_H

#include "settings.h"
#include "vector.h"

/** Struktura reprezentuj�ca scen� w kt�rej zawarte s� graficzne reprezentacje obiekt�w. */
struct Canvas
{
	Point position;					///< pozycja wzgl�dem lewego g�rnego rogu okna 
	Point size;						///< rozmiar sceny
	struct Vector* drawable_vect;	///< wektor graficznych reprezentacji obiekt�w zawartych w scenie
};

/** Funkcja inicjalizuj�ca obiekt struktury Drawable.
@param position pozycja wzgl�dem lewego g�rnego rogu okna
@param size rozmiar sceny
@return nowy obiekt */
struct Canvas* canvas_init(Point position, Point size);
/** Funkcja inicjalizuj�ca obiekt struktury Drawable przypisany do danej sceny
@param canvas scena
@param position pozycja wzgl�dem lewego g�rnego rogu sceny
@return nowy obiekt */
struct Drawable* canvas_new_drawable(struct Canvas* canvas, Point position);
/** Funkcja renderuj�ca dan� scen�.
@param canvas scena
@param r_info obiekt przechowuj�cy informacje potrzebne do wygenerowania grafiki */
void canvas_draw(struct Canvas* canvas, struct RenderInfo r_info);
/** Funkcja usuwaj�ca podany obiekt
@param canvas obiekt do usuni�cia */
void canvas_delete(struct Canvas* canvas);

#endif