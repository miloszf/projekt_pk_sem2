#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdbool.h>
#include "settings.h"

#define OUTPUT_ALREADY_SET 1

/** Struktura reprezentuj¹ca wewnêtrzne sygna³y maszyny. */
struct Signal;
/** Typ wyliczeniowy reprezentuj¹cy rodzaje sygna³ów. */
typedef enum { SIGNAL_FROM_TO, SIGNAL_ALU, SIGNAL_OTHER } SignalType;

/** Pomocnicza struktura s³u¿¹ca do inicjalizacji sygna³ów. */
struct SignalInit
{
	SignalType type;							///< typ sygna³u
	const char* name;							///< nazwa sygna³u
	struct DrawableSignalInit* drawable_init;	///< wskaŸnik na obiekt s³u¿¹cy do inicjalizacji graficznej reprezentacji sygna³u
	union
	{
		/** Struktura s³u¿¹ca do inicjalizacji sygna³u typu z-do. 
			Sygna³ pobiera wartoœæ z uk³adu wejœciowego, przetwarza j¹ podan¹ funkcj¹ 
			z wykorzystaniem maski bitowej i zapisuje do uk³adu wyjœciowego. */
		struct SignalInitFromTo
		{
			struct Unit* from;			///< uk³ad 'z'
			struct Unit* to;			///< uk³ad 'do'
			const var* mask_ptr;		///< wskaŸnik na maskê
			var(*function)(var, var);	///< wskaŸnik na funkcjê
		} from_to;

		/** Struktura s³u¿¹ca do inicjalizacji sygna³u obs³ugiwanego przez jednostkê 
			arytmetyczno-logiczn¹. Sygna³ pobiera wartoœci z dwóch uk³adów, przetwarza je 
			podan¹ funkcj¹ z wykorzystaniem maski bitowej i zapisuje do uk³adu wyjœciowego. */
		struct SignalInitALU
		{
			struct Unit* from;				///< uk³ad 'z'
			struct Unit* to;				///< uk³ad 'do'
			struct Unit* value_from;		///< drugi uk³ad z którego pobierana jest wartoœæ
			const var* mask_ptr;			///< wskaŸnik na maskê
			var(*function)(var, var, var);	///< wskaŸnik na funkcjê
		} alu;

		/** Struktura s³u¿¹ca do inicjalizacji sygna³u nie zaliczaj¹cego siê do ¿adnej z
		powy¿szych kategorii. Przechowuje wskaŸnik do obiektu przechowuj¹cego potrzebne informacje
		oraz wskaŸnik na funkcjê odpowiadaj¹c¹ danemu sygna³owi. */
		struct SignalInitOther
		{
			var(*function)(void*);	///< wskaŸnik na funkcjê
			void* value_ptr;		///< wskaŸnik na obiekt z informacjami
		} other;
	} value;
};

/** Funkcja tworz¹ca nowy sygna³ z wykorzystaniem inicjalizuj¹cego obiektu. 
@param signal_init wskaŸnik na obiekt przechowuj¹cy ustawienia\
@param nowy obiekt */
struct Signal* signal_new(struct SignalInit* signal_init);
/** Funkcja zmieniaj¹ca stan sygna³u na aktywny, wykonuje funkcjê przypisan¹ do sygna³u.
@param signal sygna³ do ustawienia
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var signal_set(struct Signal* signal);
/** Funkcja zmieniaj¹ca stan sygna³u na nieaktywny. */
void signal_reset(struct Signal* signal);
/** Funkcja zwracaj¹ca nazwê sygna³u.
@param signal sygna³
@raturn nazwa sygna³u */
const char* signal_get_name(struct Signal* signal);
/** Funkcja zmieniaj¹ca widocznoœæ sygna³u.
@param signal sygna³
@param visibility widocznoœæ sygna³u */
void signal_set_visibility(struct Signal* signal, bool visibility);
/** Funkcja ustawiaj¹ca graficzn¹ reprezentacjê sygna³u.
@param signa sygna³ */
void signal_draw(struct Signal* signal);
/** Funkcja usuwaj¹ca podany obiekt
@param signal obiekt do usuniêcia */
void signal_delete(struct Signal* signal);

#endif