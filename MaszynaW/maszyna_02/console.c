#include "console.h"
#include "graphics.h"
#include "error.h"
#include "settings.h"

struct Console
{
	wchar* buffer;
	unsigned buffer_size;
	unsigned line_width;
	unsigned line_number;
	wchar* first_char;
	wchar* last_char;
	wchar** line_array;
};

struct Console* console_init(struct Canvas* canvas, Point position, Point size)
{
	CHECK_IF_NULL(canvas);
	if ((size.x < 3) || (size.y < 3))
		CRASH_LOG(LOG_UNKNOWN_VALUE);

	struct Console* new_console = malloc_s(sizeof(struct Console));
	unsigned new_line_width = size.x - 2;
	unsigned new_line_number = size.y - 2;
	unsigned new_buffer_size = new_line_width * new_line_number;
	wchar* new_buffer = malloc_s((new_buffer_size + 1) * sizeof(wchar));
	wchar* new_first_char = new_buffer;
	wchar* new_last_char = new_first_char;
	wchar** new_line_array = malloc(new_line_number * sizeof(wchar*));
	for (unsigned i = 0; i < new_line_number; i++)
	{
		new_line_array[i] = malloc_s((new_line_width + 1) * sizeof(wchar));
		new_line_array[i][0] = '\0';
	}
	drawable_new_text_field(canvas, POINT(position.x + 1, position.y + 1), POINT(new_line_width, new_line_number), new_line_array);
	*new_console = (struct Console){ 
		new_buffer,
		new_buffer_size,
		new_line_width,
		new_line_number,
		new_first_char,
		new_last_char,
		new_line_array
	};
	drawable_new_frame(canvas, position, size);
	return new_console;
}

void console_update(struct Console* console)
{
	CHECK_IF_NULL(console);

	wchar* index = console->first_char;
	unsigned line_index = 0;
	unsigned line_array_index = 0;
	wchar** buffer_ptr_array = malloc(console->line_number * sizeof(wchar*));
	buffer_ptr_array[0] = console->first_char;
	for (unsigned i = 1; i < console->line_number; i++)
		buffer_ptr_array[i] = NULL;

	while (index < console->last_char)
	{
		if (line_index >= console->line_width || *index == '\n')
		{
			if (line_array_index < console->line_number - 1)
				line_array_index++;
			else
				for (unsigned i = 0; i < console->line_number - 1; i++)
					buffer_ptr_array[i] = buffer_ptr_array[i + 1];

			buffer_ptr_array[line_array_index] = index;
			line_index = 0;
		}

		if (*index != '\n')
			line_index++;
		index++;
	}

	for (unsigned i = 0; i < console->line_number; i++)
	{
		wchar* buffer_index = buffer_ptr_array[i];
		wchar* next_line_buff_ptr;

		if (!buffer_ptr_array[i])
			next_line_buff_ptr = buffer_index;
		else if ((i == console->line_number - 1) || (!buffer_ptr_array[i + 1]))
			next_line_buff_ptr = console->last_char;
		else
			next_line_buff_ptr = buffer_ptr_array[i + 1];

		wchar* line_ptr = console->line_array[i];
		while (buffer_index != next_line_buff_ptr)
		{
			if (*buffer_index != '\n')
				*line_ptr++ = *buffer_index;
			buffer_index++;
		}
		*line_ptr = '\0';
	}

	free(buffer_ptr_array);
}

void console_print(struct Console* console, const char* text)
{
	CHECK_IF_NULL(console);
	if (text)
	{
		while (*text)
		{
			if ((unsigned)(console->last_char - console->buffer) >= console->buffer_size)
			{
				wchar* temp_buffer = malloc_s((console->buffer_size * 2 + 1 ) * sizeof(wchar));
				memcpy(temp_buffer, console->buffer, sizeof(wchar) * console->buffer_size);
				free(console->buffer);
				console->buffer = temp_buffer;
				console->last_char = temp_buffer + (console->last_char - console->first_char);
				console->first_char = temp_buffer;
				console->buffer_size *= 2;
			}
			*console->last_char++ = *text++;
		}
		console_update(console);
	}
}

int console_get_char(struct Console* console)
{
	CHECK_IF_NULL(console);
	int chr;
	if (console->first_char == console->last_char)
		chr = EOL;
	else
	{
		chr = (char)*console->first_char++;
		console_update(console);
	}
	return chr;
}

void console_clear(struct Console* console)
{
	CHECK_IF_NULL(console);
	free(console->buffer);
	console->buffer_size = console->line_width * console->line_number;
	console->buffer = malloc_s((console->buffer_size + 1) * sizeof(wchar));
	for (unsigned i = 0; i < console->line_number; i++)
		console->line_array[i][0] = '\0';
	console->first_char = console->buffer;
	console->last_char = console->first_char;
}

void console_delete(struct Console* console)
{
	if (console)
	{
		free(console->buffer);
		for (unsigned i = 0; i < console->line_number; i++)
			free(console->line_array[i]);
		free(console->line_array);
		free(console);
	}
}