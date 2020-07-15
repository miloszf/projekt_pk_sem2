#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdbool.h>
#include "settings.h"

#define OUTPUT_ALREADY_SET 1

/** Struktura reprezentuj�ca wewn�trzne sygna�y maszyny. */
struct Signal;
/** Typ wyliczeniowy reprezentuj�cy rodzaje sygna��w. */
typedef enum { SIGNAL_FROM_TO, SIGNAL_ALU, SIGNAL_OTHER } SignalType;

/** Pomocnicza struktura s�u��ca do inicjalizacji sygna��w. */
struct SignalInit
{
	SignalType type;							///< typ sygna�u
	const char* name;							///< nazwa sygna�u
	struct DrawableSignalInit* drawable_init;	///< wska�nik na obiekt s�u��cy do inicjalizacji graficznej reprezentacji sygna�u
	union
	{
		/** Struktura s�u��ca do inicjalizacji sygna�u typu z-do. 
			Sygna� pobiera warto�� z uk�adu wej�ciowego, przetwarza j� podan� funkcj� 
			z wykorzystaniem maski bitowej i zapisuje do uk�adu wyj�ciowego. */
		struct SignalInitFromTo
		{
			struct Unit* from;			///< uk�ad 'z'
			struct Unit* to;			///< uk�ad 'do'
			const var* mask_ptr;		///< wska�nik na mask�
			var(*function)(var, var);	///< wska�nik na funkcj�
		} from_to;

		/** Struktura s�u��ca do inicjalizacji sygna�u obs�ugiwanego przez jednostk� 
			arytmetyczno-logiczn�. Sygna� pobiera warto�ci z dw�ch uk�ad�w, przetwarza je 
			podan� funkcj� z wykorzystaniem maski bitowej i zapisuje do uk�adu wyj�ciowego. */
		struct SignalInitALU
		{
			struct Unit* from;				///< uk�ad 'z'
			struct Unit* to;				///< uk�ad 'do'
			struct Unit* value_from;		///< drugi uk�ad z kt�rego pobierana jest warto��
			const var* mask_ptr;			///< wska�nik na mask�
			var(*function)(var, var, var);	///< wska�nik na funkcj�
		} alu;

		/** Struktura s�u��ca do inicjalizacji sygna�u nie zaliczaj�cego si� do �adnej z
		powy�szych kategorii. Przechowuje wska�nik do obiektu przechowuj�cego potrzebne informacje
		oraz wska�nik na funkcj� odpowiadaj�c� danemu sygna�owi. */
		struct SignalInitOther
		{
			var(*function)(void*);	///< wska�nik na funkcj�
			void* value_ptr;		///< wska�nik na obiekt z informacjami
		} other;
	} value;
};

/** Funkcja tworz�ca nowy sygna� z wykorzystaniem inicjalizuj�cego obiektu. 
@param signal_init wska�nik na obiekt przechowuj�cy ustawienia\
@param nowy obiekt */
struct Signal* signal_new(struct SignalInit* signal_init);
/** Funkcja zmieniaj�ca stan sygna�u na aktywny, wykonuje funkcj� przypisan� do sygna�u.
@param signal sygna� do ustawienia
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var signal_set(struct Signal* signal);
/** Funkcja zmieniaj�ca stan sygna�u na nieaktywny. */
void signal_reset(struct Signal* signal);
/** Funkcja zwracaj�ca nazw� sygna�u.
@param signal sygna�
@raturn nazwa sygna�u */
const char* signal_get_name(struct Signal* signal);
/** Funkcja zmieniaj�ca widoczno�� sygna�u.
@param signal sygna�
@param visibility widoczno�� sygna�u */
void signal_set_visibility(struct Signal* signal, bool visibility);
/** Funkcja ustawiaj�ca graficzn� reprezentacj� sygna�u.
@param signa sygna� */
void signal_draw(struct Signal* signal);
/** Funkcja usuwaj�ca podany obiekt
@param signal obiekt do usuni�cia */
void signal_delete(struct Signal* signal);

#endif