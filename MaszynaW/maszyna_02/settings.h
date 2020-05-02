#ifndef SETTINGS_H
#define SETTINGS_H

// Plik zawieraj¹cy ogólne informacje wykorzystywane w ró¿nych miejscach programu

#include <stdint.h>
#include <wchar.h>

#define EMPTY -1
typedef int32_t var;
typedef uint32_t u_var;
typedef wchar_t wchar;

typedef struct Point
{
	int x;
	int y;
} Point;

#define POINT(x,y) (Point){x, y}

inline Point p_add(Point a, Point b)
{
	return POINT((a.x + b.x), (a.y + b.y));
}

#endif