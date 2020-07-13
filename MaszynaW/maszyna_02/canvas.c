#include <stdlib.h>
#include "canvas.h"
#include "drawable.h"
#include "primitive.h"
#include "error.h"

struct Canvas* canvas_init(Point position, Point size)
{
	struct Canvas* canvas = malloc_s(sizeof(struct Canvas));
	*canvas = (struct Canvas){ position, size, NULL};
	canvas->drawable_vect = vector_init(sizeof(struct Drawable*));
	return canvas;
}

struct Drawable* canvas_new_drawable(struct Canvas* canvas, Point position)
{
	CHECK_IF_NULL(canvas);
	//Point zero = { 0, 0 };
	struct Drawable* new_drawable = drawable_init(position);
	struct Drawable** new_drawable_ptr = vector_push(canvas->drawable_vect, &new_drawable);
	return *new_drawable_ptr;
}

void canvas_draw(struct Canvas* canvas, struct RenderInfo r_info)
{
	CHECK_IF_NULL(canvas);
	r_info.offset = canvas->position;
	size_t drawable_num = vector_size(canvas->drawable_vect);
	for (size_t i = 0; i < drawable_num; i++)
	{
		struct Drawable** drawable_ptr = vector_read(canvas->drawable_vect, i);
		drawable_draw(*drawable_ptr, r_info);
	}
}

void canvas_delete(struct Canvas* canvas)
{
	struct Drawable** draw;
	while (draw = vector_pop(canvas->drawable_vect))
	{
		drawable_delete(*draw);
		free(draw);
	}
	vector_delete(canvas->drawable_vect);
	free(canvas);
}