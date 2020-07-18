#ifndef TERMINAL_H
#define TERMINAL_H

#include "window.h"

/** Struktura przechowuj�ca informacje zwi�zane z obs�ug� terminala. */
struct Terminal;

/** Funkcja inicjalizuj�ca obiekt struktury Terminal.
@param window_name nazwa okna terminala
@return nowy obiekt */
struct Terminal* terminal_init(const wchar* window_name);
/** Funkcja odpowiedzialna za obs�ug� grafiki terminala.
	Przepisuje bufor podanego okna do bufora terminala.
@param term terminal
@param window okno */
void terminal_display(struct Terminal* term, struct Window* window);
/** Funkcja usuwaj�ca podany obiekt.
@param terminal obiekt do usuni�cia */
void terminal_del(struct Terminal* terminal);

#endif