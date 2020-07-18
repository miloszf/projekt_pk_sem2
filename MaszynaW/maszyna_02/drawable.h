#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <stdbool.h>
#include "settings.h"
#include "vector.h"

/** Struktura b�d�ca graficzn� reprezentacj� pewnego obiektu. */
struct Drawable
{
	Point position;								///< pozycja wzgl�dem lewego g�rnego rogu sceny
	struct Vector* primitive_vect;				///< wektor prymityw�w sk�adaj�cych si� na dany obiekt
	bool is_visible;							///< widoczno��
	void* value_ptr;							///< wska�nik na obiekt zale�ny od typu rysowanego obiektu
	void(*set_value)(struct Drawable*, void*);	///< wska�nik na funkcj� zmieniaj�c� warto�� danego obiektu
};

/** Funkcja inicjalizuj�ca obiekt struktury Drawable.
@param position pozycja wzgl�dem lewego g�rnego rogu sceny
@return nowy obiekt */
struct Drawable* drawable_init(Point position);
/** Funkcja renderuj�ca dany obiekt.
@param drawable reprezentacja graficzna pewnego obiektu
@param r_info obiekt przechowuj�cy informacje potrzebne do wygenerowania grafiki */
void drawable_draw(struct Drawable* drawable, struct RenderInfo r_info);
/** Funkcja zmieniaj�ca widoczno�� danego obiektu.
@param drawable reprezentacja graficzna pewnego obiektu
@param is_visible widoczno�� */
void drawable_set_visibility(struct Drawable* drawable, bool is_visible);
/** Funkcja ustawiaj�ca warto�� danego obiektu.
@param drawable reprezentacja graficzna pewnego obiektu
@param value_ptr wska�nik na warto�� zwi�zan� z danym obiektem */
void drawable_set_value(struct Drawable* drawable, void* value_ptr);
/** Funkcja usuwaj�ca podany obiekt.
@param drawable obiekt do usuni�cia */
void drawable_delete(struct Drawable* drawable);

#endif