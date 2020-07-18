#ifndef CONSOLE_H
#define CONSOLE_H

#include "settings.h"

#define EOL -1

/** Struktura reprezentuj¹ca konsolê wejœcia/wyjœcia. Obs³uguje dopisywanie tekstu do koñca
	bufora i pobieranie pierwszego znaku. */
struct Console;

/** Funkcja inicjalizuj¹ca obiekt struktury Console.
@param canvas scena odpowiadaj¹ca danej konsoli
@param position pozycja konsoli wzglêdem lewego górnego rogu sceny
@param size rozmiar konsoli
@return wskaŸnik na nowy obiekt */
struct Console* console_init(struct Canvas* canvas, Point position, Point size);
/** Funkcja zapisuj¹ca w buforze konsoli podany tekst.
@param console konsola
@param text tekst do zapisania */
void console_print(struct Console* console, const char* text);
/** Funkcja zwracaj¹ca i usuwaj¹ca pierwszy znak z bufora konsoli.
@param console konsola
@return znak ASCII lub 'EOL' je¿eli bufor konsoli jest pusty */
int console_get_char(struct Console* console);
/** Funkcja czyszcz¹ca bufor konsoli. 
@param console konsola */
void console_clear(struct Console* console);
/** Funkcja usuwaj¹ca podan¹ konsolê. 
@param console konsola */
void console_delete(struct Console* console);

#endif