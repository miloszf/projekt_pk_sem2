#ifndef CONSOLE_H
#define CONSOLE_H

#include "settings.h"

#define EOL -1

/** Struktura reprezentuj�ca konsol� wej�cia/wyj�cia. Obs�uguje dopisywanie tekstu do ko�ca
	bufora i pobieranie pierwszego znaku. Obs�uguje znak nowej linii. **/
struct Console;

/** Funkcja inicjalizuj�ca obiekt struktury Console.
@param canvas scena odpowiadaj�ca danej konsoli
@param position pozycja konsoli wzgl�dem lewego g�rnego rogu sceny
@param size rozmiar konsoli
@return wska�nik na nowy obiekt */
struct Console* console_init(struct Canvas* canvas, Point position, Point size);
/** Funkcja zapisuj�ca w buforze konsoli podany tekst.
@param console wska�nik na konsol�
@param text tekst do zapisania */
void console_print(struct Console* console, const char* text);
/** Funkcja zwracaj�ca i usuwaj�ca pierwszy znak z bufora konsoli.
@param console wska�nik na konsol�
@return znak ASCII lub '-1' je�eli bufor konsoli jest pusty */
int console_get_char(struct Console* console);
/** Funkcja czyszcz�ca bufor konsoli. 
@param console wska�nik na konsol� */
void console_clear(struct Console* console);
/** Funkcja usuwaj�ca podan� konsol� 
@param console wska�nik na konsol� */
void console_delete(struct Console* console);

#endif