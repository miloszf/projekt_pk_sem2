#include <stdbool.h>
#include <stdlib.h>

#include "window.h"
#include "canvas.h"
#include "drawable.h"
#include "primitive.h"
#include "error.h"

#define LINE_MAP_SIZE 256 // 4*4*4*4

struct Window* window_init()
{
	struct Window* window = malloc_s(sizeof(struct Window));
	*window = (struct Window){ NULL, 0 };
	window->canvas_vect = vector_init(sizeof(struct Canvas*));
	return window;
}

struct Canvas* window_new_canvas(struct Window* window, Point position, Point size)
{
	check_for_NULL(window);
	Point new_size = { position.x + size.x, position.y + size.y };
	if (new_size.x > window->size.x || new_size.y > window->size.y)
	{
		int new_buff_length = sizeof(struct Pixel) * new_size.x * new_size.y;
		window->buffer = realloc_s(window->buffer, new_buff_length);
		window->size = new_size;
	}
	struct Canvas* new_canvas = canvas_init(position, size);
	struct Canvas** new_canvas_ptr = vector_push(window->canvas_vect, &new_canvas);
	return *new_canvas_ptr;
}

/*struct Canvas* window_new_canvas(struct Window* window, Point offset, Point new_buff_length)
{
	check_for_NULL(window);
	Point new_size = { offset.x + new_buff_length.x, offset.y + new_buff_length.y };
	if (new_size.x > window->new_buff_length.x || new_size.y > window->new_buff_length.y)
	{
		int new_buff_length = sizeof(struct Pixel) * new_size.x * new_size.y;
		window->buffer = realloc_s(window->buffer, new_buff_length);
	}
	struct Canvas new_canvas = { offset, new_buff_length, vector_init(sizeof(struct Drawable)) };
	return vector_push(window->canvas_vect, &new_canvas);
}*/

void window_draw(struct Window* window)
{
	check_for_NULL(window);
	window_clear_buffer(window);
	struct RenderInfo r_info = { window->buffer, window->size, { 0, 0 } };
	size_t canvas_num = vector_size(window->canvas_vect);
	for (size_t i = 0; i < canvas_num; i++)
	{
		struct Canvas** canvas_ptr = vector_read(window->canvas_vect, i);
		canvas_draw(*canvas_ptr, r_info);
	}
}

void window_line_to_wchar(struct Pixel* pixel)
{
	check_for_NULL(pixel);

	if (pixel->type == LINE_PRIMITIVE)
	{
		static wchar wchar_map[LINE_MAP_SIZE];
		static bool is_map_set = false;

		if (!is_map_set)
		{
			struct LineMap
			{
				wchar u_char;
				struct LineComponents line_components;
			} line_map[] = {
				{ .line_components = (struct LineComponents){0, 0, 0, 0}, .u_char = ' '}, 
				// Pojedyncze - 1 linia
				{ .line_components = (struct LineComponents){1, 0, 0, 0}, .u_char = 0x2500 },
				//{ .line_components = (struct LineComponents){1, 0, 0, 0}, .u_char = 'd' },
				//{ .line_components = (struct LineComponents){0, 0, 1, 0}, .u_char = 'b' },
				{ .line_components = (struct LineComponents){0, 0, 1, 0}, .u_char = 0x2500 },
				{ .line_components = (struct LineComponents){1, 0, 1, 0}, .u_char = 0x2500 },
				{ .line_components = (struct LineComponents){0, 1, 0, 0}, .u_char = 0x2502 },
				//{ .line_components = (struct LineComponents){0, 1, 0, 0}, .u_char = 'i' },
				{ .line_components = (struct LineComponents){0, 0, 0, 1}, .u_char = 0x2502 },
				//{ .line_components = (struct LineComponents){0, 0, 0, 1}, .u_char = 'j' },
				{ .line_components = (struct LineComponents){0, 1, 0, 1}, .u_char = 0x2502 },
				// Pojedyñcze - 2 linie
				{ .line_components = (struct LineComponents){1, 1, 0, 0}, .u_char = 0x2518 },
				{ .line_components = (struct LineComponents){0, 1, 1, 0}, .u_char = 0x2514 },
				{ .line_components = (struct LineComponents){0, 0, 1, 1}, .u_char = 0x250C },
				{ .line_components = (struct LineComponents){1, 0, 0, 1}, .u_char = 0x2510 },
				// Pojedyñcze - 3 linie           
				{ .line_components = (struct LineComponents){1, 1, 1, 0}, .u_char = 0x2534 },
				{ .line_components = (struct LineComponents){0, 1, 1, 1}, .u_char = 0x251C },
				{ .line_components = (struct LineComponents){1, 0, 1, 1}, .u_char = 0x252C },
				{ .line_components = (struct LineComponents){1, 1, 0, 1}, .u_char = 0x2524 },
				// Pojedyñcze - 4 linie           
				{ .line_components = (struct LineComponents){1, 1, 1, 1}, .u_char = 0x253C },
				// Podwójne - 1 linia             
				{ .line_components = (struct LineComponents){2, 0, 0, 0}, .u_char = 0x2550 },
				{ .line_components = (struct LineComponents){0, 0, 2, 0}, .u_char = 0x2550 },
				{ .line_components = (struct LineComponents){2, 0, 2, 0}, .u_char = 0x2550 },
				{ .line_components = (struct LineComponents){0, 2, 0, 0}, .u_char = 0x2551 },
				{ .line_components = (struct LineComponents){0, 0, 0, 2}, .u_char = 0x2551 },
				{ .line_components = (struct LineComponents){0, 2, 0, 2}, .u_char = 0x2551 },
				// Podwójne - 2 linie          
				{ .line_components = (struct LineComponents){0, 0, 2, 1}, .u_char = 0x2552 },
				{ .line_components = (struct LineComponents){0, 0, 1, 2}, .u_char = 0x2553 },
				{ .line_components = (struct LineComponents){0, 0, 2, 2}, .u_char = 0x2554 },
				{ .line_components = (struct LineComponents){2, 0, 0, 1}, .u_char = 0x2555 },
				{ .line_components = (struct LineComponents){1, 0, 0, 2}, .u_char = 0x2556 },
				{ .line_components = (struct LineComponents){2, 0, 0, 2}, .u_char = 0x2557 },
				{ .line_components = (struct LineComponents){0, 1, 2, 0}, .u_char = 0x2558 },
				{ .line_components = (struct LineComponents){0, 2, 1, 0}, .u_char = 0x2559 },
				{ .line_components = (struct LineComponents){0, 2, 2, 0}, .u_char = 0x255A },
				{ .line_components = (struct LineComponents){2, 1, 0, 0}, .u_char = 0x255B },
				{ .line_components = (struct LineComponents){1, 2, 0, 0}, .u_char = 0x255C },
				{ .line_components = (struct LineComponents){2, 2, 0, 0}, .u_char = 0x255D },
				// Podwójne - 3 linie      
				{ .line_components = (struct LineComponents){0, 1, 2, 1}, .u_char = 0x255E },
				{ .line_components = (struct LineComponents){0, 2, 1, 2}, .u_char = 0x255F },
				{ .line_components = (struct LineComponents){0, 2, 2, 2}, .u_char = 0x2560 },
				{ .line_components = (struct LineComponents){2, 1, 0, 1}, .u_char = 0x2561 },
				{ .line_components = (struct LineComponents){1, 2, 0, 2}, .u_char = 0x2562 },
				{ .line_components = (struct LineComponents){2, 2, 0, 2}, .u_char = 0x2563 },
				{ .line_components = (struct LineComponents){2, 0, 2, 1}, .u_char = 0x2564 },
				{ .line_components = (struct LineComponents){1, 0, 1, 2}, .u_char = 0x2565 },
				{ .line_components = (struct LineComponents){2, 0, 2, 2}, .u_char = 0x2566 },
				{ .line_components = (struct LineComponents){2, 1, 2, 0}, .u_char = 0x2567 },
				{ .line_components = (struct LineComponents){1, 2, 1, 0}, .u_char = 0x2568 },
				{ .line_components = (struct LineComponents){2, 2, 2, 0}, .u_char = 0x2569 },
				// Podwójne - 4 linie       
				{ .line_components = (struct LineComponents){2, 1, 2, 1}, .u_char = 0x256A },
				{ .line_components = (struct LineComponents){1, 2, 1, 2}, .u_char = 0x256B },
				{ .line_components = (struct LineComponents){2, 2, 2, 2}, .u_char = 0x256C }
			};

			// 0x2573 - big cross sign
			wmemset(wchar_map, 0x2573, LINE_MAP_SIZE);
			int line_map_size = sizeof(line_map) / sizeof(line_map[0]);
			for (int i = 0; i < line_map_size; i++)
			{
				struct LineComponents line_components = line_map[i].line_components;
				int  line_map_index =
					(line_components.left << 6) +
					(line_components.top << 4) +
					(line_components.right << 2) +
					line_components.bottom;
				wchar_map[line_map_index] = line_map[i].u_char;
			}
			is_map_set = true;
		}

		pixel->type = TEXT_PRIMITIVE;
		int  wchar_map_index =
			(pixel->line.line_components.left << 6) +
			(pixel->line.line_components.top << 4) +
			(pixel->line.line_components.right << 2) +
			pixel->line.line_components.bottom;
		pixel->u_char = wchar_map[wchar_map_index];
	}
}

void window_clear_buffer(struct Window* window)
{
	check_for_NULL(window);
	struct Pixel blank_pixel = { .type = TEXT_PRIMITIVE, .u_char = '\0', .color = COLOR_BGND_DEFAULT };
	for (int i = 0; i < (window->size.x * window->size.y); i++)
		window->buffer[i] = blank_pixel;
}

void window_delete(struct Window* window)
{
	free(window->buffer);
	struct Canvas** canvas;
	while (canvas = vector_pop(window->canvas_vect))
		canvas_delete(*canvas);
	vector_delete(window->canvas_vect);
	free(window);
}