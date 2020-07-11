#ifndef EVENT_H
#define EVENT_H

// Struktura reprezentuj¹ca pojedyñcze zdarzenie odczytane z konsoli.
// Zdarzeniem mo¿e byæ zmiana rozmiaru okna, wciœniêcie klawisza, u¿ycie myszy

#include <stdbool.h>

#include "settings.h"
#include "terminal.h"
#include "vector.h"

#define EVENT_KEY		0x01
#define EVENT_MOUSE		0x02
#define EVENT_WINDOW	0x04

// Struktura przechowuj¹ca informacjê o zmianie stanu pojedyñczego klawisza.
// key_event: 0 - 127 kod ASCII danego klawisza
//    128 - 255 klawisze specjalne 
struct KeyEvent
{
	bool key_down;
	unsigned int repeat_count;
	char key;
};

// Struktura przechowuj¹ca informacjê o zmianie stanu myszy
// scroll: 1 dla przewijania do góry
//		  -1 dla przewijania w dó³
struct MouseEvent
{
	char scroll;
};

// Struktura przechowuj¹ca informacjê o zmianie stanu okna
struct WindowEvent
{
	Point size;
};

struct InputEvent
{
	char type;
	union
	{
		struct KeyEvent key_event;
		struct MouseEvent mouse_event;
		struct WindowEvent window_event;
	};
};

// Funkcja odczytuj¹ca zdarzenia z konsoli, zwraca wektor zdarzeñ
// argumenty: wskaŸnik na terminal
struct Vector* event_get(struct Terminal* term);

#define ACSII_KEY
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