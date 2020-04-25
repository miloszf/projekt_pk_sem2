#ifndef SIGNAL_H
#define SIGNAL_H

#include "unit.h"
// Struktura reprezentuj¹ca wewnêtrzne sygna³y maszynyW.
// Sk³ada siê z nazwy sygna³u, wskaŸnika do wykonywanej funkcji i wskaŸnika
// do struktury przechowuj¹cej dane dla funkcji.
struct Signal;

typedef enum { SIGNAL_FROM_TO, SIGNAL_ALU } SignalType;
//#define MAX_STRING_LENGTH 16

struct SignalInit
{
	SignalType type;
	const char* name;
	var(*fun)(void*);
	struct DrawableSignalInit* drawable_init;
	union
	{
		struct SignalInitFromTo
		{
			struct Unit* from;
			struct Unit* to;
		} from_to;

		struct SignalInitALU
		{
			struct Unit* from;
			struct Unit* to;
			struct Unit* value_from;
		} alu;

		struct Unit** unit_array;
	} value;
};

// Funkcja tworz¹ca nowy sygna³ z podanych elementów, zwraca wskaŸnik.
// argumenty: wartoœci sk³adowe struktury
struct Signal* signal_new(struct SignalInit* signal_init);

// Funkcja wykonywuj¹ca rozkaz, zwraca okreœlon¹ wartoœæ w przypadku niepowodzenia
// wykonania przypisania.
// argumenty: wskaŸnik do struktury

// Funkcja zwracaj¹ca nazwê sygna³u
// argumenty: wskaŸnik do struktury

#endif