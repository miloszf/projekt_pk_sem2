#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "window.h"
#include "canvas.h"
#include "drawable.h"
#include "primitive.h"
#include "error.h"

#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_RED 0x0004
#define FOREGROUND_INTENSITY 0x0008
#define BACKGROUND_BLUE 0x0010
#define BACKGROUND_GREEN 0x0020
#define BACKGROUND_RED 0x0040
#define BACKGROUND_INTENSITY 0x0080

#define COLOR_DEFAULT (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define COLOR_ACTIVE (FOREGROUND_RED | FOREGROUND_INTENSITY)

#define MAX_STRING_LENGTH 16
#define MAX_REG_NAME_LENGTH 2

void add_rectangle(struct Drawable* drawable, Point position, Point size)
{
	check_for_NULL(drawable);

	struct Primitive new_line = {
		.type = LINE_PRIMITIVE,
		.position = (Point) {0, 0},
		.orientation = HORIZONTAL,
		.color = COLOR_DEFAULT,
		.line = (struct Line){.line_type = SINGLE_LINE, .length = size.x}
	};
	vector_push(drawable->primitive_vect, &new_line);
	new_line.position.y = size.y - 1;
	vector_push(drawable->primitive_vect, &new_line);
	new_line.position.y = 0;
	new_line.line.length = size.y;
	new_line.orientation = VERTICAL;
	vector_push(drawable->primitive_vect, &new_line);
	new_line.position.x = size.x - 1;
	vector_push(drawable->primitive_vect, &new_line);
}

void add_arrow(struct Canvas* canvas, Point from, Point to)
{
	check_for_NULL(canvas);
	if (from.x != to.x && from.y != to.y)
		error_set(ERROR_RENDER_FAILURE);
	else
	{
		//struct Drawable* new_drawable = canvas_new_drawable(canvas);
		//new_drawable
	}
	

}

struct ValueReg
{
	char name[MAX_STRING_LENGTH + 1];
	wchar text[2 * MAX_STRING_LENGTH + 1];
};

void value_set_reg(struct Drawable* drawable, void* value_ptr)
{
	//check_for_NULL(drawable);
	//check_for_NULL(value_ptr);
	//var value = *(var*)value_ptr;
	//struct ValueReg* value_struct = drawable->value_ptr;
	////int chars_written = swprintf_s(value_struct->text, sizeof(value_struct->text) / sizeof(*value_struct->text), L"%s:%u", value_struct->name, value);
	//size_t size = sizeof(value_struct->text) / sizeof(*value_struct->text);
	//wchar w_name[MAX_STRING_LENGTH + 1];
	//size_t
	//if (mbstowcs_s())
	//	critical_error_set("b³¹d konwercji do wchar");
	//int chars_written = swprintf_s(value_struct->text, size, L"%s:%u", value_struct->name, value);
	//if (chars_written < 0)
	//	error_set(ERROR);
}

struct Drawable* drawable_new_reg(struct Canvas* canvas, Point position, Point size, const char* name)
{
	check_for_NULL(canvas);
	check_for_NULL(name);

	struct Drawable* new_drawable;
	if (size.x && size.y)
	{
		new_drawable = canvas_new_drawable(canvas, position);
		add_rectangle(new_drawable, (Point) { 0, 0 }, size);

		struct ValueReg* new_value = malloc_s(sizeof(struct ValueReg));
		if (strncpy_s(new_value->name, MAX_STRING_LENGTH + 1, name, MAX_STRING_LENGTH))
			error_set(ERROR);
		new_drawable->value_ptr = new_value;

		struct Primitive new_text = {
		.type = TEXT_PRIMITIVE,
		.position = (Point) {1, 1},
		.orientation = HORIZONTAL,
		.color = COLOR_DEFAULT,
		.text = (struct Text){ new_value->text }
		};
		vector_push(new_drawable->primitive_vect, &new_text);
		
		new_drawable->set_value = &value_set_reg;
		var zero = 0;
		value_set_reg(new_drawable, &zero);
	}
	else
		new_drawable = NULL;

	return new_drawable;
}

struct Drawable* drawable_new_comb(struct Canvas* canvas, Point position, Point size)
{
	check_for_NULL(canvas);

	struct Drawable* new_drawable;
	if (size.x && size.y)
	{
		new_drawable = canvas_new_drawable(canvas, position);
		add_rectangle(new_drawable, (Point) { 0, 0 }, size);
	}
	else
		new_drawable = NULL;

	return new_drawable;
}

void change_color(struct Drawable* drawable, Color color)
{
	check_for_NULL(drawable);
	size_t vect_len = vector_size(drawable->primitive_vect);
	for (size_t i = 0; i < vect_len; i++)
	{
		struct Primitive* primitive = vector_read(drawable->primitive_vect, i);
		primitive->color = color;
	}
}

void value_set_bus(struct Drawable* drawable, void* value_ptr)
{
	check_for_NULL(drawable);
	check_for_NULL(value_ptr);
	var value = *(var*)value_ptr;
	if (value == EMPTY)
		change_color(drawable, COLOR_DEFAULT);
	else
		change_color(drawable, COLOR_ACTIVE);
}

struct Drawable* drawable_new_bus(struct Canvas* canvas, Point position, Point size)
{
	check_for_NULL(canvas);
	struct Drawable* new_drawable;
	if ((size.x && !size.y) || (!size.x && size.y))
	{
		new_drawable = canvas_new_drawable(canvas, position);
		struct Primitive new_line = {
		.type = LINE_PRIMITIVE,
		.position = (Point) {0, 0},
		.orientation = size.x ? HORIZONTAL : VERTICAL,
		.color = COLOR_DEFAULT,
		.line = (struct Line){.line_type = DOUBLE_LINE, .length = size.x ? size.x : size.y}
		};
		vector_push(new_drawable->primitive_vect, &new_line);
		new_drawable->set_value = &value_set_bus;
	}
	else
		new_drawable = NULL;

	return new_drawable;
}

struct ValueSignal
{
	char name[MAX_STRING_LENGTH + 1];
	struct Vector* prim_vect;
};

struct Drawable* drawable_new_signal(struct DrawableSignalInit* init)
{
	//check_for_NULL(init);
	//check_for_NULL(init->canvas);

	//struct Drawable* new_drawable;
	//if ((init->arrow.head.x == init->arrow.tail.x) && 
	//	(init->tag.head.y == init->tag.tail.y))
	//{
		//new_drawable = canvas_new_drawable(init->canvas, (Point) { 0, 0 });
		//if (init->arrow.head.y != init->arrow.tail.y)
		//{
		//	struct Primitive new_line = {
		//		.type = LINE_PRIMITIVE,
		//		.position = (init->arrow.head.y >= init->arrow.tail.y) ? init->arrow.head : init->arrow.tail,
		//		.orientation = VERTICAL,
		//		.color = COLOR_DEFAULT,
		//		.line = (struct Line){.line_type = SINGLE_LINE, .length = abs(init->arrow.head.y - init->arrow.tail.y) + 1}
		//	};
		//	vector_push(new_drawable->primitive_vect, &new_line);
		//	// Unicode for upwards arrow and downwards arrow
		//	wchar arrow_char = (init->arrow.head.y >= init->arrow.tail.y) ? 0x2191 : 0x2193;
		//	

		//	struct Primitive new_arrow = {
		//		.type = TEXT_PRIMITIVE,
		//		.position = (Point) {init->arrow.head.x, init->tag.tail.y},
		//		.orientation = HORIZONTAL,
		//		.color = COLOR_DEFAULT,
		//		.text = (struct Text){ new_value->text }
		//	};
		//}





		//new_drawable->set_value = &value_set_bus;
	//}
	//else
	//	new_drawable = NULL;

	//return new_drawable;
	return NULL;
}