#include <stdlib.h>
#include <stdbool.h>
//#include <conio.h>

#include "event_flags.h"
#include "settings.h"
#include "terminal.h"
#include "graphics.h"
#include "error.h"


int main()
{
	struct Terminal terminal;
	struct Eventflags event_flags = {0};
	bool exit = false;
	Error error = ERROR_NONE;

	CHAR_INFO ch;

	error = terminal_init(&terminal, &event_flags);

	if (error)
	{
		print_error(&error);
		return -1;
	}


	while (!exit)
	{
		error |= terminal_update(&terminal, &event_flags);

		if (error)
		{
			print_error(&error);
			exit = true;
		}
		else
		{
			if (event_flags.pressed.esc && event_flags.released.esc)
			{
				event_flags.pressed.esc = false;
				event_flags.released.esc = false;
				//exit = true;
				break;
			}

			if (event_flags.window_resize)
			{
				event_flags.window_resize = false;
				draw(&event_flags.window_size);
			}
				
		}
	}

	//_getch();

	terminal_close(&terminal);
	return 0;
}