#ifndef UNIT_H
#define UNIT_H

#include "settings.h"

// Struktura reprezentuj�ca jedn� ze sk�adowych maszynyW:
// rejestr, uk�ad kombinacyjny lub magistral�. Posiada typ, warto��, 
// wska�nik na d�ugo�� s�owa maszyny oraz sk�adow� zale�n� od typu
struct Unit;

/*
		TODO
		wzi�� poprawk� na d�ugo�� s�owa !!!!!

*/

struct UnitInit
{
	Point position;
	Point size;
	const unsigned char* word_length;
	struct Canvas* canvas;
};

// Funkcja tworz�ca rejestr, zwraca wska�nik. 
// argumenty: informacja o po�o�eniu, rozmiarze i tek�cie dla funkcji graficznych
struct Unit* unit_new_reg(struct UnitInit* init, const char* name);
// Funkcja tworz�ca uk�ad kombinacyjny, zwraca wska�nik. 
// argumenty: wska�nik do funkcji zapisuj�cej warto��
struct Unit* unit_new_comb(struct UnitInit* init);
// Funkcja tworz�ca magistral�, zwraca wska�nik. 
// argumenty: informacja o po�o�eniu, rozmiarze i tek�cie dla funkcji graficznych
struct Unit* unit_new_bus(struct UnitInit* init);

// Funkcja zapisuj�ca warto��, zwraca 0 w przypadku niepowodzenia zapisu.
// argumenty: warto�� do zapisania
var unit_set(struct Unit* unit, var value);
// Funkcja bezpo�rednio zapisuj�ca warto��
// argumenty: warto�� do zapisania
var unit_immediate_set(struct Unit* unit, var value);
// Funkcja zapisuj�ca warto�� dla rejestr�w, dla pozosta�ych nic nie robi
// argumenty: -
void unit_latch(struct Unit* unit);
// Funkcja resetuj�ca
// argumenty: -
void unit_reset(struct Unit* unit);
// Funkcja przywracaj�ca warto�ci pocz�tkowe
// argumenty: -
void unit_restart(struct Unit* unit);
// Funkcja zwracaj�ca zapisan� warto��
// argumenty: -
var unit_read(struct Unit* unit);

//void unit_draw(struct Unit* unit);
void unit_show(struct Unit* unit);
void unit_hide(struct Unit* unit);
void unit_set_visibility(struct Unit* unit, bool visibility);

void unit_delete(struct Unit* unit);

#endif