#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdbool.h>

// Takt:
// Struktura reprezentuj�ca pojedy�czy takt wykonywany przez maszyn�W.
// Posiada tablic� sygna��w, wska�nik na warunek, wska�niki na nast�pny takt
// w zale�no�ci od logicznej warto�ci warunku (if-else)
struct Tick
{
	struct Vector* signal_vect;
	bool* condition;
	struct Tick* if_true;
	struct Tick* if_false;
};

// Instruckja:
// Struktura reprezentuj�ca pojedy�czy rozkaz wykonywany przez maszyn�W.
// Posiada nazw�, liczb� argument�w, wektor/tablic� takt�w.
struct Instruction
{
	const char* name;
	unsigned char arguments;
	struct Vector* tick_vect;
};

// UWAGA: wska�nik na NULL jest traktowany jako wska�nik na czyt wys wei il
// (pierwszy takt ka�dego rozkazu)


#endif