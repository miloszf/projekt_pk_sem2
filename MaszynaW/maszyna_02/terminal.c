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

#define EVENTS_BUFFER_SIZE 128

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
} orginal_setup = { false };

void terminal_exit(void)
{
	if (!orginal_setup.init_complete)
		return;

	bool error = false;
	HANDLE in_handle = GetStdHandle(STD_INPUT_HANDLE);
	if (in_handle == INVALID_HANDLE_VALUE)
		error = true;

	if (!error)
	{
		if (!SetConsoleMode(in_handle, orginal_setup.in_mode))
			error = true;

		if (!SetConsoleActiveScreenBuffer(orginal_setup.orginal_output_handle))
			error = true;
		else if (!CloseHandle(orginal_setup.new_output_handle))
			error = true;
	}
	
	if (error)
		printf("Cannot restore cmd setup!\n");
}

struct Terminal* terminal_init(const wchar* window_name)
{
	struct Terminal* term = malloc_s(sizeof(struct Terminal));
	bool error = false;
	orginal_setup.orginal_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	term->in_handle = GetStdHandle(STD_INPUT_HANDLE);
	if (orginal_setup.orginal_output_handle == INVALID_HANDLE_VALUE || term->in_handle == INVALID_HANDLE_VALUE)
		error = true;

	if (!error)
	{
		DWORD access = GENERIC_READ | GENERIC_WRITE;
		DWORD share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		DWORD flags = CONSOLE_TEXTMODE_BUFFER;
		term->out_handle = CreateConsoleScreenBuffer(access, share_mode, NULL, flags, NULL);
		if (term->out_handle == INVALID_HANDLE_VALUE)
			error = true;
		else 
		{
			if (!SetConsoleActiveScreenBuffer(term->out_handle))
				error = true;
			else
				orginal_setup.new_output_handle = term->out_handle;
		}
	}

	if (!error)
	{
		if (!GetConsoleMode(term->in_handle, &orginal_setup.in_mode))
			error = true;
		else
		{
			DWORD console_mode = ENABLE_EXTENDED_FLAGS;
			if (!SetConsoleMode(term->in_handle, console_mode))
				error = true;
			else
			{
				console_mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
				if (!SetConsoleMode(term->in_handle, console_mode))
					error = true;
			}
		}
	}

	if (!error)
	{
		CONSOLE_CURSOR_INFO cursor_info;
		if (!GetConsoleCursorInfo(term->out_handle, &cursor_info))
			error = true;
		else
		{
			cursor_info.bVisible = false;
			if (!SetConsoleCursorInfo(term->out_handle, &cursor_info))
				error = true;
		}
	}

	if (!error)
		if (!SetConsoleTitle(window_name))
			error = true;

	if (!error)
		if (atexit(&terminal_exit))
			error = true;
		else
			orginal_setup.init_complete = true;

	if (error)
		CRASH_LOG(TERMINAL_FAILURE);
	
	return term;
}

CHAR_INFO pixel_to_char_info(const struct Pixel* pixel)
{
	wchar new_char = (pixel->type == TEXT_PRIMITIVE) ? pixel->u_char : '?';
	return (CHAR_INFO) { new_char, pixel->color };
}

void terminal_display(struct Terminal* term, struct Window* window)
{
	CHECK_IF_NULL(term);
	CHECK_IF_NULL(window);

	bool error = false;
	CONSOLE_SCREEN_BUFFER_INFO csb_info;
	COORD start = { 0, 0 };
	if (!GetConsoleScreenBufferInfo(term->out_handle, &csb_info))
		error = true;

	if (!error && (window->size.x > csb_info.dwSize.X || window->size.y > csb_info.dwSize.Y))
	{
		csb_info.dwSize = (COORD){ max(window->size.x, csb_info.dwSize.X), max(window->size.y, csb_info.dwSize.Y) };
		if (!SetConsoleScreenBufferSize(term->out_handle, csb_info.dwSize))
			error = true;
		else
		{
			SMALL_RECT new_window = { 0, 0, window->size.x - 1, window->size.y - 1};
			if (!SetConsoleWindowInfo(term->out_handle, true, &new_window))
				error = true;
		}
	}

	if (!error && (term->window_size.X != csb_info.dwSize.X) || (term->window_size.Y != csb_info.dwSize.Y))
	{
		DWORD term_buff_len = csb_info.dwSize.X * csb_info.dwSize.Y;
		DWORD unused;
		if (!FillConsoleOutputAttribute(term->out_handle, 0, term_buff_len, start, &unused))
			error = true;
		else
			if (!FillConsoleOutputCharacter(term->out_handle, (TCHAR)(' '), term_buff_len, start, &unused))
				error = true;
			else
				term->window_size = csb_info.dwSize;
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
			error = true;
		free(output_buffer);
	}

	if (error)
		CRASH_LOG(TERMINAL_FAILURE);
}

void terminal_del(struct Terminal* terminal)
{
	if (terminal)
		free(terminal);
}

/************************************************************************************************/
/****************************************** INPUT_EVENT *****************************************/
/************************************************************************************************/

#define EVENT_NONE 0

struct InputEvent event_translate(INPUT_RECORD* record)
{
	CHECK_IF_NULL(record);

	struct InputEvent in_event = { EVENT_NONE };
	switch (record->EventType)
	{
	case KEY_EVENT: 
	{
		in_event.type = EVENT_KEY;
		in_event.key_event.key_down = record->Event.KeyEvent.bKeyDown;
		in_event.key_event.repeat_count = record->Event.KeyEvent.wRepeatCount;
		if (record->Event.KeyEvent.uChar.UnicodeChar > 0 && record->Event.KeyEvent.uChar.UnicodeChar <= 127)
			in_event.key_event.key = record->Event.KeyEvent.uChar.AsciiChar;
		else if (record->Event.KeyEvent.wVirtualKeyCode >= VK_F1 && record->Event.KeyEvent.wVirtualKeyCode <= VK_F12)
			in_event.key_event.key = (record->Event.KeyEvent.wVirtualKeyCode - VK_F1) + F1_KEY;
	}
	break;
	case MOUSE_EVENT:
	{
		if (record->Event.MouseEvent.dwEventFlags & MOUSE_WHEELED)
		{
			in_event.type = EVENT_MOUSE;
			in_event.mouse_event.scroll = ((INT32)record->Event.MouseEvent.dwButtonState > 0) ? -1 : 1;
		}	
	}
	break;
	default:
		break;
	}

	return in_event;
}

struct Vector* event_get(struct Terminal* term)
{
	CHECK_IF_NULL(term);

	bool error = false;
	struct Vector* events_vect = vector_init(sizeof(struct InputEvent));
	DWORD peek_number;
	do
	{
		INPUT_RECORD peek_record;
		if (!PeekConsoleInput(term->in_handle, &peek_record, 1, &peek_number))
			error = true;
		else if (peek_number)
		{
			DWORD events_read = 0;
			do
			{
				INPUT_RECORD input_buffer[EVENTS_BUFFER_SIZE];
				if (!ReadConsoleInput(term->in_handle, input_buffer, EVENTS_BUFFER_SIZE, &events_read))
					error = true;
				else
					for (unsigned int i = 0; i < events_read; i++)
					{
						struct InputEvent input_event = event_translate(input_buffer + i);
						if (input_event.type != EVENT_NONE)
							vector_push(events_vect, &input_event);
					}
			} while (!error && events_read == EVENTS_BUFFER_SIZE);
		}
		
	} while (!error && peek_number);

	if (error)
		CRASH_LOG(TERMINAL_FAILURE);

	return events_vect;
}