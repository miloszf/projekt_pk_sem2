#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "primitive.h"
#include "error.h"

bool in_rect(Point point, Point rect)
{
	return (point.x < rect.x&& point.y < rect.y);
}

LineType line_map_type(LineType background, LineType line_type)
{
	LineType result;
	if (background == NO_LINE)
		result = line_type;
	else if (background == SINGLE_LINE)
	{
		if (line_type == SINGLE_LINE || line_type == DOUBLE_LINE)
			result = DOUBLE_LINE;
		else
			result = SINGLE_LINE;
	}
	else
	{
		result = DOUBLE_LINE;
	}
	return result;
}

struct LineChar line_char_overwrite(struct LineChar* back_char, struct LineChar* new_char)
{
	check_for_NULL(back_char);
	check_for_NULL(new_char);

	struct LineChar overwriten_char;
	//overwriten_char.line_char = new_char->line_char;
	overwriten_char.line_components.left = line_map_type(back_char->line_components.left, new_char->line_components.left);
	overwriten_char.line_components.top = line_map_type(back_char->line_components.top, new_char->line_components.top);
	overwriten_char.line_components.right = line_map_type(back_char->line_components.right, new_char->line_components.right);
	overwriten_char.line_components.bottom = line_map_type(back_char->line_components.bottom, new_char->line_components.bottom);
	return overwriten_char;
}


void primitive_draw(struct Primitive* primitive, struct RenderInfo r_info)
{
	check_for_NULL(primitive);

	r_info.offset.x += primitive->position.x;
	r_info.offset.y += primitive->position.y;

	//if (r_info.offset.x < r_info.buffer_size.x &&
	//	r_info.offset.y < r_info.buffer_size.y)
	if (in_rect(r_info.offset, r_info.buffer_size))
	{
		struct Pixel pixel;
		pixel.color = primitive->color;
		pixel.type = primitive->type;
		Point coord = { r_info.offset.x, r_info.offset.y };
		if (primitive->type == TEXT_PRIMITIVE)
		{
			for (unsigned int i = 0; i < strlen(primitive->text.string) && in_rect(coord, r_info.buffer_size); i++)
			{
				pixel.u_char = primitive->text.string[i];
				int buff_coord = coord.x + r_info.buffer_size.x * coord.y;
				r_info.buffer[buff_coord] = pixel;
				if (primitive->orientation == HORIZONTAL)
					coord.x++;
				else
					coord.y++;
			}
		}
		else if (primitive->type == LINE_PRIMITIVE)
		{
			for (unsigned int i = 0; i < primitive->line.length && in_rect(coord, r_info.buffer_size); i++)
			{
				size_t buff_coord = coord.x + r_info.buffer_size.x * coord.y;
				pixel.line = (struct LineChar){ 0 };
				struct LineChar old_line;

				if (r_info.buffer[buff_coord].type == LINE_PRIMITIVE)
					old_line = r_info.buffer[buff_coord].line;
				else
					old_line = (struct LineChar){ 0 };

				if (primitive->orientation == HORIZONTAL)
				{
					if (i > 0)
						pixel.line.line_components.left = primitive->line.line_type;

					if (i < (primitive->line.length - 1))
						pixel.line.line_components.right = primitive->line.line_type;
				}
				else
				{
					if (i > 0)
						pixel.line.line_components.top = primitive->line.line_type;

					if (i < (primitive->line.length - 1))
						pixel.line.line_components.bottom = primitive->line.line_type;
				}

				pixel.line = line_char_overwrite(&old_line, &pixel.line);
				r_info.buffer[buff_coord] = pixel;

				if (primitive->orientation == HORIZONTAL)
					coord.x++;
				else
					coord.y++;
			}
		}
		else
			error_set(ERROR_RENDER_FAILURE);
	}
}