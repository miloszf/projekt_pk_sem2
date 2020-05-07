#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "graphics.h"
#include "window.h"
#include "canvas.h"
#include "drawable.h"
#include "primitive.h"
#include "error.h"

#define MAX_STRING_LENGTH 16
#define MAX_REG_NAME_LENGTH 2

#define SIGNAL_PRIMITIVE_ARRAY_SIZE 2

#define WINDOWS_WCHAR_SPECIFIER L"S"
#define DEFAULT_WCHAR_SPECIFIER L"s"

#define MEMORY_LINE_BUFFER_SIZE (18 + 1)
#define MEMORY_LINES_NUMBER	8

void add_rectangle(struct Drawable* drawable, Point position, Point size)
{
	check_for_NULL(drawable);

	struct Primitive new_line = {
		.type = LINE_PRIMITIVE,
		.position = (Point) {0, 0},
		.orientation = HORIZONTAL,
		.color = COLOR_FGND_DEFAULT,
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

struct ValueReg
{
	char name[MAX_STRING_LENGTH + 1];
	wchar text[2 * MAX_STRING_LENGTH + 1];
};

void value_set_reg(struct Drawable* drawable, void* value_ptr)
{
	check_for_NULL(drawable);
	check_for_NULL(value_ptr);
	var value = *(var*)value_ptr;
	struct ValueReg* value_struct = drawable->value_ptr;
	size_t wbuffer_size = sizeof(value_struct->text) / sizeof(*value_struct->text);
	// WARNING!
	// currently used format modifier is Windows-specific
	// for other compilers switch to format L"%s:%u"!
	int chars_written = swprintf_s(value_struct->text, wbuffer_size, L"%" WINDOWS_WCHAR_SPECIFIER L":%u", value_struct->name, value);
	if (chars_written < 0)
		error_set(ERROR);
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
		.color = COLOR_FGND_DEFAULT,
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
		change_color(drawable, COLOR_FGND_DEFAULT);
	else
		change_color(drawable, COLOR_FGND_ACTIVE);
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
		.color = COLOR_FGND_DEFAULT,
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
	wchar arrow[2];
	wchar tag[MAX_STRING_LENGTH + 1];
	struct Primitive* primitive_ptr_array[SIGNAL_PRIMITIVE_ARRAY_SIZE];
};

void value_set_signal(struct Drawable* drawable, void* value_ptr)
{
	check_for_NULL(drawable);
	check_for_NULL(value_ptr);
	bool value = *(bool*)value_ptr;
	struct ValueSignal* value_signal = drawable->value_ptr;
	Color color = value ? COLOR_FGND_ACTIVE : COLOR_FGND_DEFAULT;
	for (int i = 0; i < SIGNAL_PRIMITIVE_ARRAY_SIZE; i++)
		if (value_signal->primitive_ptr_array[i])
			value_signal->primitive_ptr_array[i]->color = color;
}

struct Drawable* drawable_new_signal(struct DrawableSignalInit* init, const char* name)
{
	check_for_NULL(init);
	check_for_NULL(init->canvas);

	struct Drawable* new_drawable;
	if ((init->arrow.head.x == init->arrow.tail.x) && 
		(init->tag.head.y == init->tag.tail.y))
	{
		new_drawable = canvas_new_drawable(init->canvas, (Point) { 0, 0 });
		struct ValueSignal* new_value = malloc_s(sizeof(struct ValueSignal));
		for (int i = 0; i < SIGNAL_PRIMITIVE_ARRAY_SIZE; i++)
			new_value->primitive_ptr_array[i] = NULL;
		int primitive_array_index = 0;
		new_drawable->value_ptr = new_value;

		if (init->arrow.head.y != init->arrow.tail.y)
		{
			struct Primitive new_line = {
				.type = LINE_PRIMITIVE,
				.position = (init->arrow.head.y < init->arrow.tail.y) ? init->arrow.head : init->arrow.tail,
				.orientation = VERTICAL,
				.color = COLOR_FGND_DEFAULT,
				.line = (struct Line){.line_type = SINGLE_LINE, .length = abs(init->arrow.head.y - init->arrow.tail.y) + 1}
			};
			vector_push(new_drawable->primitive_vect, &new_line);

			// Unicode for upwards arrow and downwards arrow
			wchar arrow_char = (init->arrow.head.y >= init->arrow.tail.y) ? 0x2193 : 0x2191;
			new_value->arrow[0] = arrow_char;
			new_value->arrow[1] = '\0';
			struct Primitive new_arrow = {
				.type = TEXT_PRIMITIVE,
				.position = (Point) {init->arrow.head.x, init->tag.tail.y},
				.orientation = HORIZONTAL,
				.color = COLOR_FGND_DEFAULT,
				.text = (struct Text){ new_value->arrow }
			};
			//vector_push(new_drawable->primitive_vect, &new_arrow);
			new_value->primitive_ptr_array[primitive_array_index++] = vector_push(new_drawable->primitive_vect, &new_arrow);
		}

		enum {LEFT, RIGHT} tag_orientation = (init->tag.head.x > init->tag.tail.x) ? LEFT : RIGHT;
		size_t name_len = strlen(name);
		size_t tag_field_len = abs(init->tag.head.x - init->tag.tail.x) + 1;
		// name_len + '>'
		if ((name_len + 1) <= MAX_STRING_LENGTH && (name_len + 1) <= tag_field_len)
		{
			wmemset(new_value->tag, 0x2501, tag_field_len);
			new_value->tag[tag_field_len] = '\0';

			wchar tag_pointer_char;
			switch (init->tag.type)
			{
			case TO_REG_TYPE:
				tag_pointer_char = (tag_orientation == LEFT) ? 0x25BA : 0x25C4;
				break;
			case TO_COMB_TYPE:
			case TO_BUS_TYPE:
				tag_pointer_char = 0x253F;
				break;
			default:
				critical_error_set("");
				tag_pointer_char = '\0';
			}

			size_t wbuffer_size = sizeof(new_value->tag) / sizeof(*new_value->tag);
			size_t offset;
			if (tag_orientation == LEFT)
			{
				offset = 0;
				new_value->tag[tag_field_len - 1] = tag_pointer_char;
			}	
			else
			{
				offset = tag_field_len - name_len;
				new_value->tag[0] = tag_pointer_char;
			}
			//int chars_written = swprintf_s(new_value->tag + offset, wbuffer_size - offset, L"%" WINDOWS_WCHAR_SPECIFIER, name);
			//if (chars_written < 0)
				//error_set(ERROR);
			for (size_t i = 0; i < name_len; i++)
				new_value->tag[offset + i] = (wchar)(name[i]);

			Point position;
			position.x = (tag_orientation == LEFT) ? init->tag.tail.x : init->tag.head.x;
			position.y = init->tag.head.y;
			struct Primitive new_tag = {
				.type = TEXT_PRIMITIVE,
				.position = position,
				.orientation = HORIZONTAL,
				.color = COLOR_FGND_DEFAULT,
				.text = (struct Text){ new_value->tag }
			};
			//vector_push(new_drawable->primitive_vect, &new_tag);
			new_value->primitive_ptr_array[primitive_array_index++] = vector_push(new_drawable->primitive_vect, &new_tag);
		}

		new_drawable->set_value = &value_set_signal;
	}
	else
		new_drawable = NULL;

	return new_drawable;
	return NULL;
}

struct ValueMemory
{
	wchar buffer[MEMORY_LINES_NUMBER][MEMORY_LINE_BUFFER_SIZE];
	const var** memory_ptr;
	const var* addr_len;
	struct Vector** instr_names_vect_ptr;
	var offset;
};

void value_set_memory(struct Drawable* drawable, void* value_ptr)
{
	check_for_NULL(drawable);
	check_for_NULL(value_ptr);

	var offset = *(var*)value_ptr;
	struct ValueMemory* value_memory = drawable->value_ptr;
	offset += value_memory->offset;
	if (offset < 0)
		value_memory->offset = 0;
	else
	{
		var memory_max_offset = (1 << (*value_memory->addr_len)) - MEMORY_LINES_NUMBER;
		value_memory->offset = (offset > memory_max_offset) ? memory_max_offset : offset;
	}

	Error error = NO_ERROR;
	check_for_NULL(value_memory->memory_ptr);
	for (unsigned i = 0; i < MEMORY_LINES_NUMBER; i++)
	{
		var index = value_memory->offset + i;
		var value = (*value_memory->memory_ptr) ? (*value_memory->memory_ptr)[index] : 0;
		var code = (u_var)(value) >> (*value_memory->addr_len);
		char** name_ptr;
		char empty[] = "";
		char* name;
		if ((value_memory->instr_names_vect_ptr) && (name_ptr = vector_read(*value_memory->instr_names_vect_ptr, code)))
			name = *name_ptr;
		else
			name = empty;

		int chars_written = swprintf_s(value_memory->buffer[i], MEMORY_LINE_BUFFER_SIZE, L"%5u %8u %3" WINDOWS_WCHAR_SPECIFIER, index, value, name);
		if (chars_written < 0)
			error = ERROR;
	}

	if (error)
		error_set(error);
}

struct Drawable* drawable_new_memory(struct DrawableMemoryInit* init)
{
	check_for_NULL(init);
	check_for_NULL(init->canvas);
	check_for_NULL(init->memory_ptr);

	struct Drawable* new_drawable = canvas_new_drawable(init->canvas, init->position);
	struct ValueMemory* new_value = malloc_s(sizeof(struct ValueMemory));
	new_value->memory_ptr = init->memory_ptr;
	new_value->addr_len = init->addr_len;
	new_value->instr_names_vect_ptr = init->instr_names_vect_ptr;
	new_value->offset = 0;

	add_rectangle(new_drawable, (Point) { 0, 0 }, (Point) { MEMORY_LINE_BUFFER_SIZE + 1, MEMORY_LINES_NUMBER + 2 });
	
	for (unsigned i = 0; i < MEMORY_LINES_NUMBER; i++)
	{
		struct Primitive new_line = {
		.type = TEXT_PRIMITIVE,
		.position = (Point) {1, 1 + i},
		.orientation = HORIZONTAL,
		.color = COLOR_FGND_DEFAULT,
		.text = (struct Text){ new_value->buffer[i] }
		};
		vector_push(new_drawable->primitive_vect, &new_line);
	}

	new_drawable->set_value = &value_set_memory;
	new_drawable->value_ptr = new_value;
	var zero = 0;
	value_set_memory(new_drawable, &zero);

	return new_drawable;
}

struct Drawable* drawable_new_frame(struct Canvas* canvas, Point position, Point size)
{
	struct Drawable* new_frame = drawable_new_comb(canvas, position, size);
	if (new_frame)
		drawable_set_visibility(new_frame, true);
	return new_frame;
}