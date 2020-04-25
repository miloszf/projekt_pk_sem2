#ifndef SIGNAL_H
#define SIGNAL_H

#include "unit.h"
// Struktura reprezentuj�ca wewn�trzne sygna�y maszynyW.
// Sk�ada si� z nazwy sygna�u, wska�nika do wykonywanej funkcji i wska�nika
// do struktury przechowuj�cej dane dla funkcji.
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

// Funkcja tworz�ca nowy sygna� z podanych element�w, zwraca wska�nik.
// argumenty: warto�ci sk�adowe struktury
struct Signal* signal_new(struct SignalInit* signal_init);

// Funkcja wykonywuj�ca rozkaz, zwraca okre�lon� warto�� w przypadku niepowodzenia
// wykonania przypisania.
// argumenty: wska�nik do struktury

// Funkcja zwracaj�ca nazw� sygna�u
// argumenty: wska�nik do struktury

#endif