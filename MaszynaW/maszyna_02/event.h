#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>

#include "settings.h"
#include "terminal.h"
#include "vector.h"

//#define EVENT_KEY		0x01
//#define EVENT_MOUSE		0x02
//#define EVENT_WINDOW	0x04

/** Typ wyliczeniowy reprezentuj¹cy rodzaje zdarzeñ zg³aszanych przez terminal. */
typedef enum { EVENT_KEY = 0x01, EVENT_MOUSE = 0x02, EVENT_WINDOW = 0x04 } EventType;

/** Struktura przechowuj¹ca informacjê o zmianie stanu pojedyñczego klawisza. */
struct KeyEvent
{
	bool key_down;				///< 'true' - klawisz wciœniêty, 'false' - klawisz puszczony
	unsigned int repeat_count;	///< liczba powtórzeñ wciœniêcia danego klawisza
	char key;					///< kod klawisza: [0 - 127] kod ASCII, [128 - 255] klawisze specjalne
};

/** Struktura przechowuj¹ca informacjê o zmianie stanu myszy. */
struct MouseEvent
{
	char scroll;	///< '1' dla przewijania do góry, '-1' dla przewijania w dó³
};

/** Struktura przechowuj¹ca informacjê o zmianie stanu okna (nieu¿ywana) */
struct WindowEvent
{
	Point size;	///< nowy rozmiar okna
};

/** Struktura reprezentuj¹ca pojedyñcze zdarzenie odczytane z terminala.
	Zdarzeniem mo¿e byæ zmiana rozmiaru okna, wciœniêcie klawisza, u¿ycie myszy. */
struct InputEvent
{
	EventType	type;	///< typ zdarzenia
	union
	{
		struct KeyEvent key_event;			///< zdarzenie zwi¹zane z klawiatur¹
		struct MouseEvent mouse_event;		///< zdarzenie zwi¹zane z mysz¹
		struct WindowEvent window_event;	///< zdarzenie zwi¹zane z oknem terminala
	};
};

/** Funkcja odczytuj¹ca zdarzenia z konsoli, zwraca wektor zdarzeñ.
@param term wskaŸnik na terminal
@return wektor zdarzeñ */
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