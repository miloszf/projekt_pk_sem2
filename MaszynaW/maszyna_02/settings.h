#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>
#include <wchar.h>

#define EMPTY -1
typedef int32_t var;
typedef uint32_t u_var;
typedef wchar_t wchar;

/** Struktura reprezentuj�ca punkt w dwuwymiarowym uk�adzie wsp�rz�dnych. */
typedef struct Point
{
	int x;	///< wsp�rz�dna X
	int y;	///< wsp�rz�dna Y
} Point;

/** Makro "konstruktor" struktury Point. */
#define POINT(x,y) (Point){x, y}

/** Funkcja realizuj�ca dodawanie dw�ch punkt�w. 
@param a punkt A
@param b punkt B
@return suma wsp�rz�dnych punkt�w A i B */
inline Point p_add(Point a, Point b)
{
	return POINT((a.x + b.x), (a.y + b.y));
}

#endif