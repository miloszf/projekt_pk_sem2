#ifndef TERMINAL
#define TERMINAL

#include <windows.h>
#include "settings.h"
#include "error.h"

struct Terminal
{
	HANDLE output_handle;
	HANDLE input_handle;
	//CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	//COORD size;

	struct Event
	{
		enum {KEY_PRESSED, WINDOW_RESIZED} flag;
		union
		{
			enum { KEY_ESC, KEY_F1 } key;
			struct Point window_size;
		} value;
	} *events;

	unsigned events_number;
};

Error terminal_init(struct Terminal *term);
Error terminal_update(struct Terminal* term);
void terminal_close(struct Terminal* term);

#endif // !TERMINAL