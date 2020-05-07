#ifndef PRIMITIVE_H
#define PRIMITIVE_H
// Struktura przedstawiaj¹ca podstawowe graficzne obiekty.

#include "settings.h"

#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_RED 0x0004
#define FOREGROUND_INTENSITY 0x0008
#define FOREGROUND 0x000F
#define BACKGROUND_BLUE 0x0010
#define BACKGROUND_GREEN 0x0020
#define BACKGROUND_RED 0x0040
#define BACKGROUND_INTENSITY 0x0080
#define BACKGROUND 0x00F0

#define COLOR_FGND_DEFAULT 0 //(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define COLOR_FGND_ACTIVE (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define COLOR_BGND_DEFAULT (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY)

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