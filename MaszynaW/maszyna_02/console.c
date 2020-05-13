#include "console.h"
#include "graphics.h"
#include "error.h"
#include "settings.h"

struct Console
{
	wchar* buffer;
	Point buffer_size;
	wchar*** line_ptr_array;
	Point index;
	struct
	{
		// upper left / bottom right corner
		Point ul_corner;
		Point br_corner;
	} viewport;
};

void peek_array(wchar*** buff, unsigned buff_height)
{
	wchar** array[20];
	unsigned size = (buff_height < 20) ? buff_height : 20;
	for (unsigned i = 0; i < size; i++)
		array[i] = buff[i];
	return;
}

struct Console* console_init(struct Canvas* canvas, Point position, Point size)
{
	CHECK_IF_NULL(canvas);
	if ((size.x < 3) || (size.y < 3))
		CRASH_LOG(LOG_UNKNOWN_VALUE);

	struct Console* new_console = malloc_s(sizeof(struct Console));
	Point buffer_size = POINT(size.x - 1, size.y - 2);
	wchar* buffer = calloc_s(buffer_size.x * buffer_size.y, sizeof(wchar));
	wchar** temp_ptr_array = malloc_s(sizeof(wchar*) * buffer_size.y);
	wchar*** line_ptr_array = malloc_s(sizeof(wchar**) * buffer_size.y);
	for (int i = 0; i < buffer_size.y; i++)
	{
		temp_ptr_array[i] = buffer + (i * buffer_size.x);
		line_ptr_array[i] = temp_ptr_array + i;
	}
	drawable_new_text_field(canvas, POINT(position.x + 1, position.y + 1), buffer_size, line_ptr_array);
	*new_console = (struct Console){ buffer, buffer_size, line_ptr_array, POINT(0, 0), POINT(0, 0), POINT(buffer_size.x - 1, buffer_size.y) };
	drawable_new_frame(canvas, position, size);
	free(temp_ptr_array);
	return new_console;
}

inline bool in_rect(Point point, Point ul_corner, Point br_corner)
{
	return (point.x >= ul_corner.x && point.x <= br_corner.x && 
			point.y >= ul_corner.y && point.y <= br_corner.y);
}


//void scroll_buffer(wchar*** buff, unsigned buff_height)
//{
//	CHECK_IF_NULL(buff);
//	if (buff_height)
//	{
//		wchar* first_line = **buff;
//		unsigned line_index = 0;
//		for (; line_index < (buff_height - 1); line_index++)
//			*(buff[line_index]) = *(buff[line_index + 1]);
//		*(buff[line_index]) = first_line;
//	}
//}

//void scroll_buffer(wchar*** line_ptr_array, Point buffer_size, wchar* start)
//{
//	CHECK_IF_NULL(line_ptr_array);
//	for (unsigned i = 0; i < buffer_size.y; i++)
//	{
//		CHECK_IF_NULL(line_ptr_array[i]);
//		*(line_ptr_array[i]) = start + (buffer_size.x * i);
//	}
//}

void scroll_buffer(struct Console* console)
{
	CHECK_IF_NULL(console);
	int viewport_height = console->viewport.br_corner.y - console->viewport.ul_corner.y;
	wchar* start = console->buffer + (console->buffer_size.x * console->viewport.ul_corner.y);
	for (unsigned i = 0; i < viewport_height; i++)
	{
		CHECK_IF_NULL(console->line_ptr_array[i]);
		*(console->line_ptr_array[i]) = start + (console->buffer_size.x * i);
	}
}

//void scroll(struct Console* console)
//{
//	peek_array(console->line_ptr_array, console->buffer_size.y);
//	scroll_buffer(console->line_ptr_array, console->buffer_size.y);
//	peek_array(console->line_ptr_array, console->buffer_size.y);
//}

void console_print(struct Console* console, const char* text)
{
	CHECK_IF_NULL(console);
	if (!text)
		return;
	
	bool exit = false;
	while (!exit)
	{
		bool increment_index = false;
		bool scroll = false;

		if (*text && console->index.x >= (console->viewport.br_corner.x))
		{
			console->index.x = 0;
			console->index.y++;
		}
		
		if (console->index.y >= console->viewport.br_corner.y)
		{
			if (console->index.y >= console->buffer_size.y)
			{
				size_t buffer_length = console->buffer_size.x * console->buffer_size.y;
				console->buffer = realloc_s(console->buffer, sizeof(wchar) * buffer_length * 2);
				memset(console->buffer + buffer_length, '\0', sizeof(wchar) * buffer_length);
				console->buffer_size.y *= 2;
			}
			console->viewport.ul_corner.y++;
			console->viewport.br_corner.y++;

			//int viewport_height = console->viewport.br_corner.y - console->viewport.ul_corner.y;
			//wchar* start = console->buffer + (console->buffer_size.x * console->viewport.ul_corner.y);
			//scroll_buffer(console->line_ptr_array, POINT(console->buffer_size.x, viewport_height), start);
			scroll_buffer(console);
		}

		int index = console->index.x + (console->index.y * console->buffer_size.x);
		console->buffer[index] = (wchar)*text;

		if (*text)
		{
			console->index.x++;
			text++;
		}
		else
			exit = true;
	}
}

char console_get_char(struct Console* console)
{
	CHECK_IF_NULL(console);
	char chr = (char)(*console->buffer);
	if (chr)
	{
		wchar* copy_index = console->buffer;
		for (int line_index = 0; line_index < console->buffer_size.y; line_index++)
		{
			//int index = console->buffer_size.x * line_index;
			//int i = 0;
			//for (; i < console->buffer_size.x - 2; i++)
			//	console->buffer[index + i] = console->buffer[index + i + 1];
			//if (line_index < (console->buffer_size.y - 1))
			//	console->buffer[index + i] = console->buffer[console->buffer_size.x * (line_index + 1)];
			//else
			//	console->buffer[index + i] = '\0';
			wchar* index = console->buffer + console->buffer_size.x * line_index;
			while (*(index + 1))
				*index++ = *(index + 1);
			if (line_index < (console->buffer_size.y - 1))
				*index = console->buffer[console->buffer_size.x * (line_index + 1)];
			else
				*index = '\0';
		}
		if (console->index.x)
			console->index.x--;
		else if (console->index.y)
		{
			console->index.x = console->viewport.br_corner.x - 1;
			console->index.y--;
		}

		if (console->index.y < (console->viewport.br_corner.y - 1))
		{
			console->viewport.ul_corner.y--;
			console->viewport.br_corner.y--;
			scroll_buffer(console);
		}
	}

	return chr;
}


void console_clear(struct Console* console)
{
	CHECK_IF_NULL(console);
	size_t buffer_size = sizeof(wchar) * console->buffer_size.x * console->buffer_size.y;
	memset(console->buffer, '\0', buffer_size);
	console->viewport.br_corner.y -= console->viewport.ul_corner.y;
	console->viewport.ul_corner.y = 0;
	console->index = POINT(0, 0);
	scroll_buffer(console);
}

void console_delete(struct Console* console)
{
	if (console)
	{
		free(console->buffer);
		free(console->line_ptr_array);
		free(console);
	}
}