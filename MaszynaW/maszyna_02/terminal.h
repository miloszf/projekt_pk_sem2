#ifndef TERMINAL_H
#define TERMINAL_H

#include "window.h"

/** Struktura przechowuj¹ca informacje zwi¹zane z obs³ug¹ terminala */
struct Terminal;

/** Funkcja inicjalizuj¹ca obiekt struktury Terminal.
@param window_name nazwa okna terminala
@return nowy obiekt */
struct Terminal* terminal_init(const wchar* window_name);
/** Funkcja odpowiedzialna za obs³ugê grafiki terminala.
	Przepisuje bufor podanego okna do bufora terminala.
@param term terminal
@param window Ÿród³owe okno */
void terminal_display(struct Terminal* term, struct Window* window);
/** Funkcja usuwaj¹ca podany obiekt
@param terminal obiekt do usuniêcia */
void terminal_del(struct Terminal* terminal);

#endif