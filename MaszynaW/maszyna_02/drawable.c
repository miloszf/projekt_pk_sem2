#include <stdlib.h>

#include "drawable.h"
#include "primitive.h"
#include "canvas.h"
#include "error.h"

struct Drawable* drawable_init(Point position)
{
	struct Drawable* drawable = malloc_s(sizeof(struct Drawable));
	*drawable = (struct Drawable){ position, NULL, false, NULL, NULL };
	drawable->primitive_vect = vector_init(sizeof(struct Primitive));
	return drawable;
}

void drawable_draw(struct Drawable* drawable, struct RenderInfo r_info)
{
	CHECK_IF_NULL(drawable);

	if (drawable->is_visible)
	{
		r_info.offset.x += drawable->position.x;
		r_info.offset.y += drawable->position.y;
		size_t primitive_num = vector_size(drawable->primitive_vect);
		for (size_t i = 0; i < primitive_num; i++)
		{
			struct Primitive* primitive = vector_read(drawable->primitive_vect, i);
			primitive_draw(primitive, r_info);
		}
	}
}

void drawable_set_visibility(struct Drawable* drawable, bool is_visible)
{
	CHECK_IF_NULL(drawable);
	drawable->is_visible = is_visible;
}

void drawable_set_value(struct Drawable* drawable, void* value_ptr)
{
	if (!drawable)
		CHECK_IF_NULL(drawable);
	if (drawable->set_value)
		drawable->set_value(drawable, value_ptr);
}

void drawable_delete(struct Drawable* drawable)
{
	if (drawable)
	{
		vector_delete(drawable->primitive_vect);
		free(drawable->value_ptr);
		free(drawable);
	}
}