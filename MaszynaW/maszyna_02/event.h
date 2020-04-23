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
// scroll: > 0 dla przewijania do góry
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
#define F1_KEY		0x01
#define F2_KEY		0x02
#define F3_KEY		0x03
#define F4_KEY		0x04
#define F5_KEY		0x05
#define F6_KEY		0x06
#define F7_KEY		0x07
#define F8_KEY		0x08
#define F9_KEY		0x09
#define F10_KEY		0x0A
#define F11_KEY		0x0B
#define F12_KEY		0x0C
#define ESC_KEY		0x1B

#endif