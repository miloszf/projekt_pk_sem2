#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#include "settings.h"
#include "terminal.h"
#include "graphics.h"
#include "error.h"
#include "event_flags.h"


int main()
{
	struct Terminal terminal;
	bool exit = false;
	Error error = ERROR_NONE;

	error = terminal_init(&terminal);

	if (error) 
	{
		print_error(&error);
		return -1;
	}

	
	while (!exit)
	{
		error = terminal_update(&terminal);

		if (error)
		{
			print_error(&error);
			exit = true;
			
		}
		else
		{
			bool wresize_flag = false;

			while (terminal.events_number)
			{

			}
			//if (terminal.event.flag)
			//{
			//	switch (terminal.event.flag)
			//	{
			//	case KEY_PRESSED:
			//		switch (terminal.event.value.key)
			//		{
			//		case KEY_ESC:
			//			exit = true;
			//			break;
			//		}
			//		break;
			//	case WINDOW_RESIZED:
			//		draw(terminal.event.value.window_size.x, terminal.event.value.window_size.y);
			//		break;
			//	}
			//}
		}
	}

	//_getch();

	terminal_close(&terminal);
	return 0;
}