#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdbool.h>

// Takt:
// Struktura reprezentuj¹ca pojedyñczy takt wykonywany przez maszynêW.
// Posiada tablicê sygna³ów, wskaŸnik na warunek, wskaŸniki na nastêpny takt
// w zale¿noœci od logicznej wartoœci warunku (if-else)
struct Tick
{
	struct Vector* signal_vect;
	bool* condition;
	struct Tick* if_true;
	struct Tick* if_false;
};

// Instruckja:
// Struktura reprezentuj¹ca pojedyñczy rozkaz wykonywany przez maszynêW.
// Posiada nazwê, liczbê argumentów, wektor/tablicê taktów.
struct Instruction
{
	const char* name;
	unsigned char arguments;
	struct Vector* tick_vect;
};

// UWAGA: wskaŸnik na NULL jest traktowany jako wskaŸnik na czyt wys wei il
// (pierwszy takt ka¿dego rozkazu)


#endif