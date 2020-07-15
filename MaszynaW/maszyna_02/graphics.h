#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include "settings.h"
#include "color.h"

/** Struktura reprezentująca okno w którym zawarte są sceny. Rozmiar okna dopasowuje się automatycznie
	do zawartych w nim scen. */
struct Window;
/** Struktura reprezentująca scenę w której zawarte są graficzne reprezentacje obiektów. */
struct Canvas;
/** Struktura będąca graficzną reprezentacją pewnego obiektu. */
struct Drawable;

/** Typ wyliczeniowy reprezentujący typy graficznych reprezentacji sygnałów. */
typedef enum { TO_REG_TYPE, TO_COMB_TYPE, TO_BUS_TYPE } DrawableSignalType;
/** Pomocnicza struktura służąca do inicjalizacji graficznej reprezentacji sygnałów. */
struct DrawableSignalInit
{
	struct Canvas* canvas;	///< scena w której zawiera się dany obiekt
	/** Struktura reprezentująca strzałkę łączącą dwa układy. */
	struct
	{
		Point head;	///< grot strzałki
		Point tail;	///< koniec strzałki
	} arrow;
	/** Struktura reprezentująca etykietę przypisaną do danego sygnału. */
	struct
	{
		Point head;					///< początek etykiety
		Point tail;					///< koniec etykiety			
		DrawableSignalType type;	///< typ sygnału
	} tag;
};
/** Pomocnicza struktura służąca do inicjalizacji graficznej reprezentacji pamięci maszyny. */
struct DrawableMemoryInit
{
	struct Canvas* canvas;					///< scena w której zawiera się dany obiekt
	Point position;							///< pozycja względem lewego górnego rogu sceny 
	const var** memory_ptr;					///< wskaźnik na początek pamięci maszyny
	const var* addr_len;					///< wskaźnik na zmienną przechowującą liczbę bitów adresowych
	struct Vector** instr_names_vect_ptr;	/// wskaźnik na wektor instrukcji
};

// Opisane w window.h
struct Window* window_init();
struct Canvas* window_new_canvas(struct Window* window, Point offset, Point size);
void window_draw(struct Window* window);
void window_delete(struct Window* window);
// Opisane w drawable.h
void drawable_set_visibility(struct Drawable* drawable, bool is_visible);
void drawable_set_value(struct Drawable* drawable, void* value_ptr);

/** Funkcja zwracająca graficzną reprezentację rejestru.
@param canvas scena w której zawiera się dany obiekt
@param position pozycja względem lewego górnego rogu sceny 
@param size rozmiar rejestru
@param name nazwa rejestru 
@return nowy obiekt */
struct Drawable* drawable_new_reg(struct Canvas* canvas, Point position, Point size, const char* name);
struct Drawable* drawable_new_comb(struct Canvas* canvas, Point position, Point size);
/** Funkcja zwracająca graficzną reprezentację magistrali.
@param canvas scena w której zawiera się dany obiekt
@param position pozycja względem lewego górnego rogu sceny
@param size rozmiar rejestru
@return nowy obiekt */
struct Drawable* drawable_new_bus(struct Canvas* canvas, Point position, Point size);
/** Funkcja zwracająca graficzną reprezentację sygnału.
@param init wskaźnik na obiekt zawierający informacje potrzebne do inicjalizacji nowego obiektu
@param name nazwa rejestru
@return nowy obiekt */
struct Drawable* drawable_new_signal(struct DrawableSignalInit* init, const char* name);
/** Funkcja zwracająca graficzną reprezentację pamięci.
@param init wskaźnik na obiekt zawierający informacje potrzebne do inicjalizacji nowego obiektu
@return nowy obiekt */
struct Drawable* drawable_new_memory(struct DrawableMemoryInit* init);
/** Funkcja zwracająca graficzną reprezentację ramki.
@param canvas scena w której zawiera się dany obiekt
@param size rozmiar ramki
@return nowy obiekt */
struct Drawable* drawable_new_frame(struct Canvas* canvas, Point position, Point size);
/** Funkcja zwracająca graficzną reprezentację pola tekstowego.
@param canvas scena w której zawiera się dany obiekt
@param position pozycja względem lewego górnego rogu sceny
@param size rozmiar ramki
@param line_array wskaźnik na tablicę linii z tekstem
@return nowy obiekt */
struct Drawable* drawable_new_text_field(struct Canvas* canvas, Point position, Point size, wchar** line_array);
/** Funkcja zwracająca graficzną reprezentację przycisku.
@param canvas scena w której zawiera się dany obiekt
@param position pozycja względem lewego górnego rogu sceny
@param size rozmiar ramki
@param text tekst wyświetlany na przycisku
@param color_set zestaw kolorów dla różnych stanów przycisku
@return nowy obiekt */
struct Drawable* drawable_new_button(struct Canvas* canvas, Point position, Point size, const char* text, struct ColorSet* color_set);

#endif