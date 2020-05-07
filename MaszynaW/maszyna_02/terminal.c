#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

#include "terminal.h"
#include "event.h"
#include "error.h"
#include "settings.h"
#include "window.h"
#include "primitive.h"

//#define WCHAR_BUFFER_SIZE 256
#define EVENTS_BUFFER_SIZE 128

//#define ESC "\x1b"
//#define CSI "\x1b["
//
//void inline enter_alternate_buffer()
//{
//	printf(CSI "?1049h");
//}
//
//void inline exit_alternate_buffer()
//{
//	printf(CSI "?1049l");
//}
//
//void inline hide_cursor()
//{
//	printf(CSI "?25l");
//}
//
//void inline show_cursor()
//{
//	printf(CSI "?25h");
//}

struct Terminal
{
	HANDLE out_handle;
	HANDLE in_handle;
	COORD window_size;
};

struct ConsoleSetup
{
	bool init_complete;
	DWORD in_mode;
	DWORD out_mode;
	HANDLE orginal_output_handle;
	HANDLE new_output_handle;
	//CONSOLE_CURSOR_INFO cursor_info;
	//WCHAR title[WCHAR_BUFFER_SIZE];
} orginal_setup = { false };

void terminal_exit(void)
{
	if (!orginal_setup.init_complete)
		return;
	
	//exit_alternate_buffer();
	//show_cursor();

	Error error = NO_ERROR;
	//HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE in_handle = GetStdHandle(STD_INPUT_HANDLE);
	if (/*out_handle == INVALID_HANDLE_VALUE ||*/
		in_handle == INVALID_HANDLE_VALUE)
		error = ERROR;

	if (!error)
	{
		//if (!SetConsoleMode(out_handle, orginal_setup.out_mode))
			//error_get = ERROR;

		if (!SetConsoleMode(in_handle, orginal_setup.in_mode))
			error = ERROR;

		//SetConsoleTitle(orginal_setup.title);
		if (!SetConsoleActiveScreenBuffer(orginal_setup.orginal_output_handle))
			error = ERROR;
		else if (!CloseHandle(orginal_setup.new_output_handle))
			error = ERROR;
	}
	
	if (error)
		printf("B³¹d podczas przywracania pocz¹tkowych ustawieñ terminala.\n");
}

struct Terminal* terminal_init(const char* window_name)
{
	struct Terminal* term = malloc_s(sizeof(struct Terminal));

	Error error = NO_ERROR;
	orginal_setup.orginal_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	term->in_handle = GetStdHandle(STD_INPUT_HANDLE);
	if (orginal_setup.orginal_output_handle == INVALID_HANDLE_VALUE ||
		term->in_handle == INVALID_HANDLE_VALUE)
		error = ERROR;

	// Output init
	if (!error)
	{
		DWORD access = GENERIC_READ | GENERIC_WRITE;
		DWORD share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		DWORD flags = CONSOLE_TEXTMODE_BUFFER;
		term->out_handle = CreateConsoleScreenBuffer(access, share_mode, NULL, flags, NULL);
		if (term->out_handle == INVALID_HANDLE_VALUE)
			error = ERROR;
		else 
		{
			//COORD max_window_size = GetLargestConsoleWindowSize(term->out_handle);
			//if (!max_window_size.X || !max_window_size.Y)
			//	error_get = ERROR;
			//else if (!SetConsoleScreenBufferSize(term->out_handle, max_window_size))
			//	error_get = ERROR;
			//else 
			if (!SetConsoleActiveScreenBuffer(term->out_handle))
				error = ERROR;
			else
				orginal_setup.new_output_handle = term->out_handle;
		}
		
	}

	//if (!error_get)
	//{
	//	if (!GetConsoleMode(term->out_handle, &orginal_setup.out_mode))
	//		error = ERROR;
	//	else
	//	{
	//		DWORD console_mode =
	//			orginal_setup.out_mode |
	//			//ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	//			DISABLE_NEWLINE_AUTO_RETURN;
	//		if (!SetConsoleMode(term->out_handle, console_mode))
	//			error = ERROR;
	//	}
	//}
	
	if (!error)
	{
		if (!GetConsoleMode(term->in_handle, &orginal_setup.in_mode))
			error = ERROR;
		else
		{
			DWORD console_mode =
				orginal_setup.in_mode |
				//ENABLE_WINDOW_INPUT |
				ENABLE_MOUSE_INPUT;
			if (!SetConsoleMode(term->in_handle, console_mode))
				error = ERROR;
		}
	}

	if (!error)
	{
		CONSOLE_CURSOR_INFO cursor_info;
		if (!GetConsoleCursorInfo(term->out_handle, &cursor_info))
			error = ERROR;
		else
		{
			//orginal_setup.cursor_info = cursor_info;
			cursor_info.bVisible = false;
			if (!SetConsoleCursorInfo(term->out_handle, &cursor_info))
				error = ERROR;
		}
	}
		
	//WCHAR console_title[WCHAR_BUFFER_SIZE];

	if (!error)
	{
		if (atexit(&terminal_exit))
			error = ERROR;
		else
		{
			orginal_setup.init_complete = true;
		}	
	}

	if (error)
		error_set(ERROR_TERMINAL_FAILURE);
	
	return term;
}

CHAR_INFO pixel_to_char_info(const struct Pixel* pixel)
{
	wchar new_char = (pixel->type == TEXT_PRIMITIVE) ? pixel->u_char : '?';
	return (CHAR_INFO) { new_char, pixel->color };
}

void terminal_display(struct Terminal* term, struct Window* window)
{
	check_for_NULL(term);
	check_for_NULL(window);

	Error error = NO_ERROR;
	CONSOLE_SCREEN_BUFFER_INFO csb_info;
	COORD start = { 0, 0 };

	if (!GetConsoleScreenBufferInfo(term->out_handle, &csb_info))
		error = ERROR;

	if (!error && (term->window_size.X != csb_info.dwSize.X || term->window_size.Y != csb_info.dwSize.Y))
	{
		DWORD term_buff_len = csb_info.dwSize.X * csb_info.dwSize.Y;
		DWORD unused;
		if (!FillConsoleOutputAttribute(term->out_handle, 0, term_buff_len, start, &unused))
			error = ERROR;
		else
		{
			DWORD term_buff_len = csb_info.dwSize.X * csb_info.dwSize.Y;
			DWORD unused;
			if (!FillConsoleOutputCharacter(term->out_handle, (TCHAR)(' '), term_buff_len, start, &unused))
				error = ERROR;
			else
			{
				term->window_size.X = csb_info.dwSize.X;
				term->window_size.Y = csb_info.dwSize.Y;
			}
		}

		if (!error)
		{
			CONSOLE_CURSOR_INFO cursor_info;
			if (!GetConsoleCursorInfo(term->out_handle, &cursor_info))
				error = ERROR;
			else
			{
				cursor_info.bVisible = false;
				if (!SetConsoleCursorInfo(term->out_handle, &cursor_info))
					error = ERROR;
			}
		}
	}

	if (!error)
	{
		COORD window_size = { window->size.x , window->size.y };
		CHAR_INFO* output_buffer = malloc_s(sizeof(CHAR_INFO) * window_size.X * window_size.Y);
		for (int col = 0; col < window_size.Y; col++)
			for (int row = 0; row < window_size.X; row++)
			{
				window_line_to_wchar(&window->buffer[row + window_size.X * col]);
				output_buffer[row + window_size.X * col] = pixel_to_char_info(&window->buffer[row + window_size.X * col]);
			}

		SMALL_RECT write_region = { start.X, start.Y , window_size.X, window_size.Y };
		if (!WriteConsoleOutput(term->out_handle, output_buffer, window_size, start, &write_region))
			error = ERROR;
		free(output_buffer);
	}

	if (error)
		error_set(ERROR_RENDER_FAILURE);
	//Sleep(200);
}

void terminal_del(struct Terminal* terminal)
{
	if (terminal)
	{
		free(terminal);
	}
}

/*************************************************************************************************/
/****************************************** INPUT_EVENT ******************************************/
/*************************************************************************************************/

struct InputEvent event_translate(INPUT_RECORD* record)
{
	check_for_NULL(record);

	struct InputEvent in_event = {0};
	switch (record->EventType)
	{
	case KEY_EVENT: {
			in_event.type = EVENT_KEY;
			in_event.key_event.key_down = record->Event.KeyEvent.bKeyDown;
			in_event.key_event.repeat_count = record->Event.KeyEvent.wRepeatCount;
			if (record->Event.KeyEvent.uChar.UnicodeChar > 0 && record->Event.KeyEvent.uChar.UnicodeChar <= 127)
				in_event.key_event.key = record->Event.KeyEvent.uChar.AsciiChar;
			else if (record->Event.KeyEvent.wVirtualKeyCode >= VK_F1 && record->Event.KeyEvent.wVirtualKeyCode >= VK_F12)
				in_event.key_event.key = (record->Event.KeyEvent.wVirtualKeyCode - VK_F1) + F1_KEY + CONTROL_KEY;
		}
		break;
	
	}

	return in_event;
}

struct Vector* event_get(struct Terminal* term)
{
	check_for_NULL(term);

	Error error = NO_ERROR;
	struct Vector* events_vect = vector_init(sizeof(struct InputEvent));
	DWORD peek_number;
	do
	{
		INPUT_RECORD peek_record;
		if (!PeekConsoleInput(term->in_handle, &peek_record, 1, &peek_number))
			error = ERROR;
		else if (peek_number)
		{
			DWORD events_read = 0;
			do
			{
				INPUT_RECORD input_buffer[EVENTS_BUFFER_SIZE];
				if (!ReadConsoleInput(term->in_handle, input_buffer, EVENTS_BUFFER_SIZE, &events_read))
					error = ERROR;
				else
					for (unsigned int i = 0; i < events_read; i++)
					{
						struct InputEvent input_event = event_translate(input_buffer + i);
						vector_push(events_vect, &input_event);
					}
			} while (!error && events_read == EVENTS_BUFFER_SIZE);
		}
		
	} while (!error && peek_number);
	

	if (error)
		error_set(ERROR_EVENTS_FAILURE);

	return events_vect;
}