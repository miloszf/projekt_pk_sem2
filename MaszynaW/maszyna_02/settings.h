#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include <wchar.h>

#define EMPTY -1
typedef int32_t var;
typedef uint32_t u_var;
typedef wchar_t wchar;

/** Struktura reprezentuj¹ca punkt dwuwymiarowego uk³adu wspó³rzêdnych. **/
typedef struct Point
{
	int x;	///< wspó³rzêdna X
	int y;	///< wspó³rzêdna Y
} Point;

/** Makro "konstruktor" struktury Point. **/
#define POINT(x,y) (Point){x, y}

/** Funkcja realizuj¹ca dodawanie dwóch punktów. 
@param a punkt A
@param b punkt B
@return suma wspó³rzêdnych punktów A i B **/
inline Point p_add(Point a, Point b)
{
	return POINT((a.x + b.x), (a.y + b.y));
}

#endif