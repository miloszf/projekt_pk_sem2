#ifndef UNIT_H
#define UNIT_H

#include "settings.h"

/** Struktura reprezentuj�ca jeden ze sk�adowych uk�ad�w maszyny:
	rejestr, uk�ad kombinacyjny lub magistral�. */
struct Unit;

/** Pomocnicza struktura s�u��ca do inicjalizacji uk�ad�w. */
struct UnitInit
{
	Point position;						///< pozycja uk�adu wzgl�dem lewego g�rnego rogu sceny
	Point size;							///< rozmiar uk�adu
	const unsigned char* word_length;	///< wska�nik na zmienn� przechowuj�c� d�ugo�� s�owa maszyny
	struct Canvas* canvas;				///< scena
};

/** Funkcja tworz�ca nowy rejestr. 
@param init wska�nik na obiekt przechowuj�cy ustawienia pocz�tkowe
@param name nazwa rejsetru
@return nowy obiekt */
struct Unit* unit_new_reg(struct UnitInit* init, const char* name);
/** Funkcja tworz�ca nowy uk�ad kombinacyjny.
@param init wska�nik na obiekt przechowuj�cy ustawienia pocz�tkowe
@return nowy obiekt */
struct Unit* unit_new_comb(struct UnitInit* init);
/** Funkcja tworz�ca now� magistral�.
@param init wska�nik na obiekt przechowuj�cy ustawienia pocz�tkowe
@return nowy obiekt */
struct Unit* unit_new_bus(struct UnitInit* init);

/** Funkcja zapisuj�ca podan� warto�� do uk�adu. 
@param unit uk�ad
@param value warto�� do zapisania
@return '0' przy niepowodzeniu zapisu, inna warto�� w przeciwnym przypadku */
var unit_set(struct Unit* unit, var value);
/** Funkcja zapisuj�ca bezpo�rednio podan� warto�� do uk�adu.
@param unit uk�ad
@param value warto�� do zapisania
@return 'EMPTY' je�eli nadpisywany uk�ad by� pusty, inna warto�� w przeciwnym przypadku */
var unit_immediate_set(struct Unit* unit, var value);
/** Funkcja zatrzaskuj�ca warto�� rejestr�w, dla pozosta�ych uk�adu�w nie robi nic.
@param unit uk�ad */
void unit_latch(struct Unit* unit);
/** Funkcja zeruj�ca podany uk�ad.
@param unit uk�ad */
void unit_reset(struct Unit* unit);
/** Funkcja przywracaj�ca ustawienia pocz�tkowe danego uk�adu.
@param unit uk�ad */
void unit_restart(struct Unit* unit);
/** Funkcja zwracaj�ca warto�� zapisan� w uk�adzie.
@param unit uk�ad
@return warto�� zapisana w uk�adzie */
var unit_read(struct Unit* unit);
/** Funkcja ustawiaj�ca widoczno�� uk�adu
@param unit uk�ad
@param visibility widoczno��*/
void unit_set_visibility(struct Unit* unit, bool visibility);
/** Funkcja usuwaj�ca podany obiekt
@param signal obiekt do usuni�cia */
void unit_delete(struct Unit* unit);

#endif