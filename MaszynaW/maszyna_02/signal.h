#ifndef SIGNAL_H
#define SIGNAL_H

#include "unit.h"
// Struktura reprezentuj¹ca wewnêtrzne sygna³y maszynyW.
// Sk³ada siê z nazwy sygna³u, wskaŸnika do wykonywanej funkcji i wskaŸnika
// do struktury przechowuj¹cej dane dla funkcji.
struct Signal;

typedef enum { SIGNAL_FROM_TO, SIGNAL_ALU } SignalType;


struct SignalInit
{
	SignalType type;
	const char* name;
	var(*fun)(void*);
	struct DrawableInitSignal* drawable_init;
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

// Funkcja wykonywuj¹ca rozkaz, zwraca okreœlon¹ wartoœæ w przypadku niepowodzenia
// wykonania przypisania.
// argumenty: wskaŸnik do struktury

// Funkcja zwracaj¹ca nazwê sygna³u
// argumenty: wskaŸnik do struktury

#endif