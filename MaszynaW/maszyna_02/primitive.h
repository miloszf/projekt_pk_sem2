#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "settings.h"
#include "color.h"

/** Typ wyliczeniowy reprezentuj¹cy typy prymitywów graficznych. */
typedef enum { TEXT_PRIMITIVE, LINE_PRIMITIVE } PrimitiveType;
/** Typ wyliczeniowy reprezentuj¹cy orientacjê prymitywu. */
typedef enum { VERTICAL, HORIZONTAL } Orientation;
/** Typ wyliczeniowy reprezentuj¹cy rodzaj linii. */
typedef enum {
	NO_LINE = 0,
	SINGLE_LINE = 1,
	DOUBLE_LINE = 2,
} LineType;

/** Struktura przedstawiaj¹ca tekst. */
struct Text
{
	const wchar* string;	///< ³añcuch szerokich znaków 
};
/** Struktura przedstawiaj¹ca liniê. */
struct Line
{
	unsigned int length;	///< d³ugoœæ linii
	LineType line_type;		///< typ linii
};
/** Struktura reprezentuj¹ca prymityw graficzny. */
struct Primitive
{
	PrimitiveType type;			///< typ prymitywu
	Point position;				///< pozycja wzglêdem lewego górnego rogu rysowalnego obiektu
	Orientation orientation;	///< orientacja prymitywu
	Color color;				///< kolor prymitywu
	union {						
		struct Text text;		///< prymityw typu "tekst"
		struct Line line;		///< prymityw typu "linia"
	};
};
/** Struktura reprezentuj¹ca znak przedstawiaj¹cy liniê. */
struct LineChar
{
	/**Struktura reprezentuj¹ca podzia³ znaku na 4 czêœci patrz¹c od œrodka. */
	struct LineComponents
	{
		LineType left : 2;		///< typ lewej czêœci znaku
		LineType top : 2;		///< typ górnej czêœci znaku
		LineType right : 2;		///< typ prawj czêœci znaku
		LineType bottom : 2;	///< typ dolnej czêœci znaku
	} line_components;
};
/** Struktura reprezentuj¹ca pojedyñczy piksel. */
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
	struct Pixel* const buffer;	///< bufor graficzny z³o¿ony z pikseli
	const Point buffer_size;	///< rozmiar bufora
	Point offset;				///< przesuniêcie wzglêdem lewego górnego rogu bufora
};
/** Funkcja renderuj¹ca prymityw do bufora.
@param primitive prumityw
@param r_info obiekt przechowuj¹cy informacje potrzebne do wygenerowania grafiki */
void primitive_draw(struct Primitive* primitive, struct RenderInfo r_info);

#endif