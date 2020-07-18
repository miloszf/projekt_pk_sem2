#ifndef UNIT_H
#define UNIT_H

#include "settings.h"

/** Struktura reprezentuj¹ca jeden ze sk³adowych uk³adów maszyny:
	rejestr, uk³ad kombinacyjny lub magistralê. */
struct Unit;

/** Pomocnicza struktura s³u¿¹ca do inicjalizacji uk³adów. */
struct UnitInit
{
	Point position;						///< pozycja uk³adu wzglêdem lewego górnego rogu sceny
	Point size;							///< rozmiar uk³adu
	const unsigned char* word_length;	///< wskaŸnik na zmienn¹ przechowuj¹c¹ d³ugoœæ s³owa maszyny
	struct Canvas* canvas;				///< scena
};

/** Funkcja tworz¹ca nowy rejestr. 
@param init wskaŸnik na obiekt przechowuj¹cy ustawienia pocz¹tkowe
@param name nazwa rejsetru
@return nowy obiekt */
struct Unit* unit_new_reg(struct UnitInit* init, const char* name);
/** Funkcja tworz¹ca nowy uk³ad kombinacyjny.
@param init wskaŸnik na obiekt przechowuj¹cy ustawienia pocz¹tkowe
@return nowy obiekt */
struct Unit* unit_new_comb(struct UnitInit* init);
/** Funkcja tworz¹ca now¹ magistralê.
@param init wskaŸnik na obiekt przechowuj¹cy ustawienia pocz¹tkowe
@return nowy obiekt */
struct Unit* unit_new_bus(struct UnitInit* init);

/** Funkcja zapisuj¹ca podan¹ wartoœæ do uk³adu. 
@param unit uk³ad
@param value wartoœæ do zapisania
@return '0' przy niepowodzeniu zapisu, 'EMPTY' w przeciwnym przypadku */
var unit_set(struct Unit* unit, var value);
/** Funkcja wymuszaj¹ca natychmiastowy zapis wartoœci do uk³adu.
@param unit uk³ad
@param value wartoœæ do zapisania
@return 'EMPTY' je¿eli nadpisywany uk³ad by³ pusty, '0' w przeciwnym przypadku */
var unit_immediate_set(struct Unit* unit, var value);
/** Funkcja zatrzaskuj¹ca wartoœci rejestrów, dla pozosta³ych uk³adów nie robi nic.
@param unit uk³ad */
void unit_latch(struct Unit* unit);
/** Funkcja zeruj¹ca podany uk³ad.
@param unit uk³ad */
void unit_reset(struct Unit* unit);
/** Funkcja przywracaj¹ca ustawienia pocz¹tkowe danego uk³adu.
@param unit uk³ad */
void unit_restart(struct Unit* unit);
/** Funkcja zwracaj¹ca wartoœæ zapisan¹ w uk³adzie.
@param unit uk³ad
@return wartoœæ zapisana w uk³adzie lub 'EMPTY' jeœli uk³ad jest pusty */
var unit_read(struct Unit* unit);
/** Funkcja ustawiaj¹ca widocznoœæ uk³adu
@param unit uk³ad
@param visibility widocznoœæ */
void unit_set_visibility(struct Unit* unit, bool visibility);
/** Funkcja usuwaj¹ca podany obiekt
@param unit obiekt do usuniêcia */
void unit_delete(struct Unit* unit);

#endif