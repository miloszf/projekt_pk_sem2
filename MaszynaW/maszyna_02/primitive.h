#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "settings.h"
#include "color.h"

/** Typ wyliczeniowy reprezentuj�cy typy prymityw�w graficznych. */
typedef enum { TEXT_PRIMITIVE, LINE_PRIMITIVE } PrimitiveType;
/** Typ wyliczeniowy reprezentuj�cy orientacj� prymitywu. */
typedef enum { VERTICAL, HORIZONTAL } Orientation;
/** Typ wyliczeniowy reprezentuj�cy rodzaj linii. */
typedef enum {
	NO_LINE = 0,
	SINGLE_LINE = 1,
	DOUBLE_LINE = 2,
} LineType;

/** Struktura przedstawiaj�ca tekst. */
struct Text
{
	const wchar* string;	///< �a�cuch szerokich znak�w 
};
/** Struktura przedstawiaj�ca lini�. */
struct Line
{
	unsigned int length;	///< d�ugo�� linii
	LineType line_type;		///< typ linii
};
/** Struktura reprezentuj�ca prymityw graficzny. */
struct Primitive
{
	PrimitiveType type;			///< typ prymitywu
	Point position;				///< pozycja wzgl�dem lewego g�rnego rogu rysowalnego obiektu
	Orientation orientation;	///< orientacja prymitywu
	Color color;				///< kolor prymitywu
	union {						
		struct Text text;		///< prymityw typu "tekst"
		struct Line line;		///< prymityw typu "linia"
	};
};
/** Struktura reprezentuj�ca znak przedstawiaj�cy lini�. */
struct LineChar
{
	/**Struktura reprezentuj�ca podzia� znaku na 4 cz�ci patrz�c od �rodka. */
	struct LineComponents
	{
		LineType left : 2;		///< typ lewej cz�ci znaku
		LineType top : 2;		///< typ g�rnej cz�ci znaku
		LineType right : 2;		///< typ prawj cz�ci znaku
		LineType bottom : 2;	///< typ dolnej cz�ci znaku
	} line_components;
};
/** Struktura reprezentuj�ca pojedy�czy piksel. */
struct Pixel {
	PrimitiveType type;			///< typ prymitywu
	union {
		wchar u_char;			///< znak
		struct LineChar line;	///< linia
	};
	Color color;				///< kolor prymitywu
};
/** Pomocnicza struktura wykorzystywana przy renderowaniu grafiki. */
struct RenderInfo
{
	struct Pixel* const buffer;	///< bufor graficzny z�o�ony z pikseli
	const Point buffer_size;	///< rozmiar bufora
	Point offset;				///< przesuni�cie wzgl�dem lewego g�rnego rogu bufora
};
/** Funkcja renderuj�ca prymityw do bufora.
@param primitive prumityw
@param r_info obiekt przechowuj�cy informacje potrzebne do wygenerowania grafiki */
void primitive_draw(struct Primitive* primitive, struct RenderInfo r_info);

#endif