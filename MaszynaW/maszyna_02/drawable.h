#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <stdbool.h>

//#include "primitive.h"
#include "settings.h"
#include "vector.h"

// Struktura Drawable przechowuj�ca dane do p�niejszego
// narysowania danego elementu. Posiada warto�ci:
// po�o�enie, wektor prymityw�w
struct Drawable
{
	Point position;
	Point size;
	struct Vector* primitive_vect;
	bool is_visible;
};

struct Drawable* drawable_init(Point position, Point size);
void drawable_draw(struct Drawable* drawable, struct RenderInfo r_info);
void drawable_delete(struct Drawable* drawable);

void drawable_new_test(struct Canvas* canvas, Point position, Point size);

#endif