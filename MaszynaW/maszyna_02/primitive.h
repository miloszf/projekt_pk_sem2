#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "settings.h"
#include "color.h"

/** Typ wyliczeniowy reprezentujący typy prymitywów graficznych. */
typedef enum { TEXT_PRIMITIVE, LINE_PRIMITIVE } PrimitiveType;
/** Typ wyliczeniowy reprezentujący orientację prymitywu. */
typedef enum { VERTICAL, HORIZONTAL } Orientation;
/** Typ wyliczeniowy reprezentujący rodzaje linii. */
typedef enum {
	NO_LINE = 0,
	SINGLE_LINE = 1,
	DOUBLE_LINE = 2,
} LineType;

/** Struktura przedstawiająca tekst. */
struct Text
{
	const wchar* string;	///< łańcuch szerokich znaków 
};
/** Struktura przedstawiająca linię. */
struct Line
{
	unsigned int length;	///< długość linii
	LineType line_type;		///< typ linii
};
/** Struktura reprezentująca prymityw graficzny. */
struct Primitive
{
	PrimitiveType type;			///< typ prymitywu
	Point position;				///< pozycja względem lewego górnego rogu rysowalnego obiektu
	Orientation orientation;	///< orientacja prymitywu
	Color color;				///< kolor prymitywu
	union {						
		struct Text text;		///< prymityw typu "tekst"
		struct Line line;		///< prymityw typu "linia"
	};
};
/** Struktura reprezentująca znak przedstawiający linię. */
struct LineChar
{
	/** Struktura reprezentująca podział znaku na 4 części patrząc od środka. */
	struct LineComponents
	{
		// unsigned char instead of LineType to ensure that the line type is an unsigned number
		unsigned char left : 2;		///< typ lewej części znaku
		unsigned char top : 2;		///< typ górnej części znaku
		unsigned char right : 2;	///< typ prawj części znaku
		unsigned char bottom : 2;	///< typ dolnej części znaku
	} line_components;
};
/** Struktura reprezentująca pojedyńczy piksel. */
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
	struct Pixel* const buffer;	///< bufor graficzny złożony z pikseli
	const Point buffer_size;	///< rozmiar bufora
	Point offset;				///< przesunięcie względem lewego górnego rogu bufora
};
/** Funkcja renderująca prymityw do bufora.
@param primitive prumityw
@param r_info obiekt przechowujący informacje potrzebne do wygenerowania grafiki */
void primitive_draw(struct Primitive* primitive, struct RenderInfo r_info);

#endif