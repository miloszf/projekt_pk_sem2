#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>

#include "settings.h"
#include "terminal.h"
#include "vector.h"

//#define EVENT_KEY		0x01
//#define EVENT_MOUSE		0x02
//#define EVENT_WINDOW	0x04

/** Typ wyliczeniowy reprezentuj�cy rodzaje zdarze� zg�aszanych przez terminal. */
typedef enum { EVENT_KEY = 0x01, EVENT_MOUSE = 0x02, EVENT_WINDOW = 0x04 } EventType;

/** Struktura przechowuj�ca informacj� o zmianie stanu pojedy�czego klawisza. */
struct KeyEvent
{
	bool key_down;				///< 'true' - klawisz wci�ni�ty, 'false' - klawisz zwolniony
	unsigned int repeat_count;	///< liczba powt�rze� wci�ni�cia danego klawisza
	char key;					///< kod klawisza: [0 - 127] kod ASCII, [128 - 255] klawisze specjalne
};

/** Struktura przechowuj�ca informacj� o zmianie stanu myszy. */
struct MouseEvent
{
	char scroll;	///< '1' dla przewijania do g�ry, '-1' dla przewijania w d�
};

/** Struktura przechowuj�ca informacj� o zmianie stanu okna (nieu�ywana) */
struct WindowEvent
{
	Point size;	///< nowy rozmiar okna
};

/** Struktura reprezentuj�ca pojedy�cze zdarzenie odczytane z terminala.
	Zdarzeniem mo�e by� zmiana rozmiaru okna, wci�ni�cie klawisza, u�ycie myszy. */
struct InputEvent
{
	EventType type;							///< typ zdarzenia
	union
	{
		struct KeyEvent key_event;			///< zdarzenie zwi�zane z klawiatur�
		struct MouseEvent mouse_event;		///< zdarzenie zwi�zane z mysz�
		struct WindowEvent window_event;	///< zdarzenie zwi�zane z oknem terminala
	};
};

/** Funkcja odczytuj�ca zdarzenia z konsoli, zwraca wektor zdarze�.
@param term wska�nik na terminal
@return wektor zdarze� */
struct Vector* event_get(struct Terminal* term);

#define CONTROL_KEY 0x80
#define F1_KEY	(char)(0x01 | CONTROL_KEY)
#define F2_KEY	(char)(0x02 | CONTROL_KEY)
#define F3_KEY	(char)(0x03 | CONTROL_KEY)
#define F4_KEY	(char)(0x04 | CONTROL_KEY)
#define F5_KEY	(char)(0x05 | CONTROL_KEY)
#define F6_KEY	(char)(0x06 | CONTROL_KEY)
#define F7_KEY	(char)(0x07 | CONTROL_KEY)
#define F8_KEY	(char)(0x08 | CONTROL_KEY)
#define F9_KEY	(char)(0x09 | CONTROL_KEY)
#define F10_KEY	(char)(0x0A | CONTROL_KEY)
#define F11_KEY	(char)(0x0B | CONTROL_KEY)
#define F12_KEY	(char)(0x0C | CONTROL_KEY)
#define ENTER_KEY 13
#define ESC_KEY	27

#endif