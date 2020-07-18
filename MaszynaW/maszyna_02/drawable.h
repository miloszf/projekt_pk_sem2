#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <stdbool.h>
#include "settings.h"
#include "vector.h"

/** Struktura bêd¹ca graficzn¹ reprezentacj¹ pewnego obiektu. */
struct Drawable
{
	Point position;								///< pozycja wzglêdem lewego górnego rogu sceny
	struct Vector* primitive_vect;				///< wektor prymitywów sk³adaj¹cych siê na dany obiekt
	bool is_visible;							///< widocznoœæ
	void* value_ptr;							///< wskaŸnik na obiekt zale¿ny od typu rysowanego obiektu
	void(*set_value)(struct Drawable*, void*);	///< wskaŸnik na funkcjê zmieniaj¹c¹ wartoœæ danego obiektu
};

/** Funkcja inicjalizuj¹ca obiekt struktury Drawable.
@param position pozycja wzglêdem lewego górnego rogu sceny
@return nowy obiekt */
struct Drawable* drawable_init(Point position);
/** Funkcja renderuj¹ca dany obiekt.
@param drawable reprezentacja graficzna pewnego obiektu
@param r_info obiekt przechowuj¹cy informacje potrzebne do wygenerowania grafiki */
void drawable_draw(struct Drawable* drawable, struct RenderInfo r_info);
/** Funkcja zmieniaj¹ca widocznoœæ danego obiektu.
@param drawable reprezentacja graficzna pewnego obiektu
@param is_visible widocznoœæ */
void drawable_set_visibility(struct Drawable* drawable, bool is_visible);
/** Funkcja ustawiaj¹ca wartoœæ danego obiektu.
@param drawable reprezentacja graficzna pewnego obiektu
@param value_ptr wskaŸnik na wartoœæ zwi¹zan¹ z danym obiektem */
void drawable_set_value(struct Drawable* drawable, void* value_ptr);
/** Funkcja usuwaj¹ca podany obiekt.
@param drawable obiekt do usuniêcia */
void drawable_delete(struct Drawable* drawable);

#endif