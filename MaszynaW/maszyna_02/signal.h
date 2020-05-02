#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdbool.h>
#include "settings.h"
//#include "unit.h"

#define OUTPUT_ALREADY_SET 1

// Struktura reprezentuj�ca wewn�trzne sygna�y maszynyW.
// Sk�ada si� z nazwy sygna�u, wska�nika do wykonywanej funkcji i wska�nika
// do struktury przechowuj�cej dane dla funkcji.
struct Signal;

typedef enum { SIGNAL_FROM_TO, SIGNAL_ALU, SIGNAL_OTHER } SignalType;
//#define MAX_STRING_LENGTH 16

struct SignalInit
{
	SignalType type;
	const char* name;
	//var(*fun)(void*);
	struct DrawableSignalInit* drawable_init;
	union
	{
		struct SignalInitFromTo
		{
			struct Unit* from;
			struct Unit* to;
			const var* mask_ptr;
			var(*function)(var, var);
		} from_to;

		struct SignalInitALU
		{
			struct Unit* from;
			struct Unit* to;
			struct Unit* value_from;
			const var* mask_ptr;
			var(*function)(var, var, var);
		} alu;

		struct SignalInitOther
		{
			var(*function)(void*);
			void* value_ptr;
		} other;

		unsigned char* dummy;
	} value;
};

// Funkcja tworz�ca nowy sygna� z podanych element�w, zwraca wska�nik.
// argumenty: warto�ci sk�adowe struktury
struct Signal* signal_new(struct SignalInit* signal_init);

// Funkcja wykonywuj�ca rozkaz, zmienia stan sygna�u na aktywny, zwracana warto��:
// 0 - przy powodzeniu
// EMPTY - kiedy wej�ciowy unit jest pusty
// OUTPUT_ALREADY_SET - docelowy unit jest ju� zaj�ty
// argumenty: wska�nik do struktury
var signal_set(struct Signal* signal);

// Funkcja zmieniaj�ca stan sygna�u na nieaktywny
void signal_reset(struct Signal* signal);

// Funkcja zwracaj�ca nazw� sygna�u
// argumenty: wska�nik do struktury
const char* signal_get_name(struct Signal* signal);

// Funkcja zmieniaj�ca widoczno�� sygna�u
void signal_set_visibility(struct Signal* signal, bool visibility);

// Funkcja "rysuj�ca" sygna�
void signal_draw(struct Signal* signal);

void signal_delete(struct Signal* signal);

#endif