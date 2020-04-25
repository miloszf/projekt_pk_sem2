#ifndef PRIMITIVE_H
#define PRIMITIVE_H
// Struktura przedstawiaj¹ca podstawowe graficzne obiekty.

#include "settings.h"

typedef enum { TEXT_PRIMITIVE, LINE_PRIMITIVE } PrimitiveType;
typedef enum { VERTICAL, HORIZONTAL } Orientation;
typedef enum {
	NO_LINE = 0,
	SINGLE_LINE = 1,
	DOUBLE_LINE = 2,
} LineType;

typedef unsigned int Color;

struct Text
{
	const wchar* string;
};

struct Line
{
	unsigned int length;
	LineType line_type;
};

struct Primitive
{
	PrimitiveType type;
	Point position;
	Orientation orientation;
	Color color;
	union {
		struct Text text;
		struct Line line;
	};
};

struct LineChar
{
	//wchar line_char;
	//union {
		struct LineComponents
		{
			unsigned int left : 2;
			unsigned int top : 2;
			unsigned int right : 2;
			unsigned int bottom : 2;
		} line_components;
		//unsigned int line_component_index;
	//};
};

struct Pixel {
	PrimitiveType type;
	union {
		wchar u_char;
		struct LineChar line;
	};
	Color color;
};

struct RenderInfo
{
	struct Pixel* const buffer;
	const Point buffer_size;
	Point offset;
};

void primitive_draw(struct Primitive* primitive, struct RenderInfo r_info);

#endif