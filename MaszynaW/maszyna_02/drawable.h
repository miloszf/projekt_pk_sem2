#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <stdbool.h>

//#include "primitive.h"
#include "settings.h"
#include "vector.h"

// Struktura Drawable przechowująca dane do późniejszego
// narysowania danego elementu. Posiada wartości:
// położenie, wektor prymitywów
struct Drawable
{
	Point position;
	struct Vector* primitive_vect;
	bool is_visible;
	void* value_ptr;
	void(*set_value)(struct Drawable*, void*);
};

struct Drawable* drawable_init(Point position);
void drawable_draw(struct Drawable* drawable, struct RenderInfo r_info);
void drawable_set_visibility(struct Drawable* drawable, bool is_visible);
void drawable_set_value(struct Drawable* drawable, void* value_ptr);
void drawable_delete(struct Drawable* drawable);

#endif