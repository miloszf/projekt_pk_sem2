#ifndef COLOR_H
#define COLOR_H

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

#define COLOR_FGND_DEFAULT 0
#define COLOR_FGND_ACTIVE (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define COLOR_BGND_DEFAULT (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY)
#define COLOR_DEFAULT (COLOR_FGND_DEFAULT | COLOR_BGND_DEFAULT)
#define COLOR_ACTIVE (COLOR_FGND_ACTIVE | COLOR_BGND_DEFAULT)

/** Typ s�u��cy do przechowywania koloru. */
typedef unsigned int Color;

/** Struktura przechowuj�ca zestaw kolor�w. */
struct ColorSet
{
	Color f_default;	///< domy�lny kolor pierwszoplanowy
	Color b_default;	///< domy�lny kolor t�a
	Color f_active;		///< kolor pierwszoplanowy element�w aktywnych
	Color b_active;		///< kolor t�a element�w aktywnych
};

#endif