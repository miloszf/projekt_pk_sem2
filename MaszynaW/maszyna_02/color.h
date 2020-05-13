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

#define COLOR_FGND_DEFAULT 0 //(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
//#define COLOR_FGND_DEFAULT (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define COLOR_FGND_ACTIVE (FOREGROUND_RED | FOREGROUND_INTENSITY)
//#define COLOR_FGND_ACTIVE (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define COLOR_BGND_DEFAULT (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY)
//#define COLOR_BGND_DEFAULT (BACKGROUND_BLUE)

typedef unsigned int Color;

struct ColorSet
{
	Color f_default;
	Color b_default;
	Color f_active;
	Color b_active;
};

#endif