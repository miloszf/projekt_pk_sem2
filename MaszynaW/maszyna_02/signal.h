#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdbool.h>
#include "settings.h"
//#include "unit.h"

#define OUTPUT_ALREADY_SET 1

// Struktura reprezentuj¹ca wewnêtrzne sygna³y maszynyW.
// Sk³ada siê z nazwy sygna³u, wskaŸnika do wykonywanej funkcji i wskaŸnika
// do struktury przechowuj¹cej dane dla funkcji.
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

// Funkcja tworz¹ca nowy sygna³ z podanych elementów, zwraca wskaŸnik.
// argumenty: wartoœci sk³adowe struktury
struct Signal* signal_new(struct SignalInit* signal_init);

// Funkcja wykonywuj¹ca rozkaz, zmienia stan sygna³u na aktywny, zwracana wartoœæ:
// 0 - przy powodzeniu
// EMPTY - kiedy wejœciowy unit jest pusty
// OUTPUT_ALREADY_SET - docelowy unit jest ju¿ zajêty
// argumenty: wskaŸnik do struktury
var signal_set(struct Signal* signal);

// Funkcja zmieniaj¹ca stan sygna³u na nieaktywny
void signal_reset(struct Signal* signal);

// Funkcja zwracaj¹ca nazwê sygna³u
// argumenty: wskaŸnik do struktury
const char* signal_get_name(struct Signal* signal);

// Funkcja zmieniaj¹ca widocznoœæ sygna³u
void signal_set_visibility(struct Signal* signal, bool visibility);

// Funkcja "rysuj¹ca" sygna³
void signal_draw(struct Signal* signal);

void signal_delete(struct Signal* signal);

#endif