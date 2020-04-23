#include "graphics.h"
#include "window.h"
#include "canvas.h"
#include "drawable.h"
#include "primitive.h"
#include "error.h"

void add_rectangle(struct Canvas* canvas, Point position, Point size)
{
	check_for_NULL(canvas);
	struct Drawable* new_drawable = canvas_new_drawable(canvas);
	new_drawable->position = position;
	new_drawable->size = size;
	struct Primitive new_line = {
		.type = LINE_PRIMITIVE,
		.position = (Point) {0, 0},
		.orientation = HORIZONTAL,
		.color = 0x000F,
		.line = (struct Line){.line_type = SINGLE_LINE, .length = size.x}
	};
	vector_push(new_drawable->primitive_vect, &new_line);
	new_line.position.y = size.y - 1;
	vector_push(new_drawable->primitive_vect, &new_line);
	new_line.position.y = 0;
	new_line.line.length = size.y;
	new_line.orientation = VERTICAL;
	vector_push(new_drawable->primitive_vect, &new_line);
	new_line.position.x = size.x - 1;
	vector_push(new_drawable->primitive_vect, &new_line);
}

void add_arrow(struct Canvas* canvas, Point from, Point to)
{
	check_for_NULL(canvas);
	if (from.x != to.x && from.y != to.y)
		error_set(ERROR_RENDER_FAILURE);
	else
	{
		struct Drawable* new_drawable = canvas_new_drawable(canvas);
		//new_drawable
	}
	

}