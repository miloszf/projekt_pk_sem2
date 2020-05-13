#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>
#include "settings.h"
#include "color.h"

struct Window;
struct Canvas;
struct Drawable;

typedef enum { TO_REG_TYPE, TO_COMB_TYPE, TO_BUS_TYPE } DrawableSignalType;

struct DrawableSignalInit
{
	struct Canvas* canvas;
	struct
	{
		Point head;
		Point tail;
	} arrow;

	struct
	{
		Point head;
		Point tail;
		DrawableSignalType type;
	} tag;
};

struct DrawableMemoryInit
{
	struct Canvas* canvas;
	Point position;
	// depends on MEMORY_LINE_BUFFER_SIZE & MEMORY_LINES_NUMBER, graphics.c
	// Point size;
	const var** memory_ptr;
	const var* addr_len;
	struct Vector** instr_names_vect_ptr;
};

struct Window* window_init();
struct Canvas* window_new_canvas(struct Window* window, Point offset, Point size);
void window_draw(struct Window* window);
void window_delete(struct Window* window);

void drawable_set_visibility(struct Drawable* drawable, bool is_visible);
void drawable_set_value(struct Drawable* drawable, void* value_ptr);

struct Drawable* drawable_new_reg(struct Canvas* canvas, Point position, Point size, const char* name);
struct Drawable* drawable_new_comb(struct Canvas* canvas, Point position, Point size);
struct Drawable* drawable_new_bus(struct Canvas* canvas, Point position, Point size);

struct Drawable* drawable_new_signal(struct DrawableSignalInit* init, const char* name);
struct Drawable* drawable_new_memory(struct DrawableMemoryInit* init);

struct Drawable* drawable_new_frame(struct Canvas* canvas, Point position, Point size);
struct Drawable* drawable_new_text_field(struct Canvas* canvas, Point position, Point size, wchar*** line_ptr_array);
struct Drawable* drawable_new_button(struct Canvas* canvas, Point position, Point size, const char* text, struct ColorSet* color_set);

#endif