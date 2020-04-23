#ifndef CANVAS_H
#define CANVAS_H

#include "settings.h"
#include "vector.h"

// Struktura przechowuj¹ca informacje o danym wycinku ekranu:
// position, rozmiar, wektor "rysowalnych" obiektów
struct Canvas
{
	Point position;
	Point size;
	struct Vector* drawable_vect;
};

struct Canvas* canvas_init(Point position, Point size);
void canvas_draw(struct Canvas* canvas, struct RenderInfo r_info);
void canvas_delete(struct Canvas* canvas);

struct Drawable* canvas_new_drawable(struct Canvas* canvas);

#endif