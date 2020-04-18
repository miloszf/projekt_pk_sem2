#ifndef EVENT_H
#define EVENT_H

// Struktura reprezentuj¹ca pojedyñcze zdarzenie odczytane z konsoli.
// Zdarzeniem mo¿e byæ zmiana rozmiaru okna, wciœniêcie klawisza, u¿ycie myszy

#include <stdbool.h>

#include "settings.h"
#include "terminal.h"
#include "vector.h"

#define KEY_EVENT		0x01
#define MOUSE_EVENT		0x02
#define WINDOW_EVENT	0x04

struct KeyEvent;
struct MouseEvent;
struct WindowEvent;

struct InputEvent
{
	char event_type;
	union
	{
		struct KeyEvent key;
		struct MouseEvent mouse;
		struct WindowEvent window;
	} event;
};

// Funkcja odczytuj¹ca zdarzenia z konsoli, zwraca wektor zdarzeñ
// argumenty: wskaŸnik na terminal
struct Vector* event_get(struct Terminal* term);

#define CONTROL_KEY (0x80)
#define F1_KEY		(0x00 | CONTROL_KEY)
#define F2_KEY		(0x00 | CONTROL_KEY)
#define F3_KEY		(0x00 | CONTROL_KEY)
#define F4_KEY		(0x00 | CONTROL_KEY)
#define F5_KEY		(0x00 | CONTROL_KEY)
#define F6_KEY		(0x00 | CONTROL_KEY)
#define F7_KEY		(0x00 | CONTROL_KEY)
#define F8_KEY		(0x00 | CONTROL_KEY)
#define F9_KEY		(0x00 | CONTROL_KEY)
#define F10_KEY		(0x00 | CONTROL_KEY)
#define F11_KEY		(0x00 | CONTROL_KEY)
#define F12_KEY		(0x00 | CONTROL_KEY)

// Struktura przechowuj¹ca informacjê o zmianie stanu pojedyñczego klawisza.
// key: 0 - 127 kod ASCII danego klawisza
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

#endif