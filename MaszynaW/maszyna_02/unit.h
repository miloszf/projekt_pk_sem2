#ifndef UNIT_H
#define UNIT_H

#include "settings.h"

// Struktura reprezentuj¹ca jedn¹ ze sk³adowych maszynyW:
// rejestr, uk³ad kombinacyjny lub magistralê. Posiada typ, wartoœæ, 
// wskaŸnik na d³ugoœæ s³owa maszyny oraz sk³adow¹ zale¿n¹ od typu
struct Unit;

/*
		TODO
		wzi¹æ poprawkê na d³ugoœæ s³owa !!!!!

*/

struct UnitInit
{
	Point position;
	Point size;
	const unsigned char* word_length;
	struct Canvas* canvas;
};

// Funkcja tworz¹ca rejestr, zwraca wskaŸnik. 
// argumenty: informacja o po³o¿eniu, rozmiarze i tekœcie dla funkcji graficznych
struct Unit* unit_new_reg(struct UnitInit* init, const char* name);
// Funkcja tworz¹ca uk³ad kombinacyjny, zwraca wskaŸnik. 
// argumenty: wskaŸnik do funkcji zapisuj¹cej wartoœæ
struct Unit* unit_new_comb(struct UnitInit* init);
// Funkcja tworz¹ca magistralê, zwraca wskaŸnik. 
// argumenty: informacja o po³o¿eniu, rozmiarze i tekœcie dla funkcji graficznych
struct Unit* unit_new_bus(struct UnitInit* init);

// Funkcja zapisuj¹ca wartoœæ, zwraca 0 w przypadku niepowodzenia zapisu.
// argumenty: wartoœæ do zapisania
var unit_set(struct Unit* unit, var value);
// Funkcja zapisuj¹ca wartoœæ dla rejestrów, dla pozosta³ych nic nie robi
// argumenty: -
void unit_latch(struct Unit* unit);
// Funkcja resetuj¹ca
// argumenty: -
void unit_reset(struct Unit* unit);
// Funkcja przywracaj¹ca wartoœci pocz¹tkowe
// argumenty: -
void unit_restart(struct Unit* unit);
// Funkcja zwracaj¹ca zapisan¹ wartoœæ
// argumenty: -
var unit_read(struct Unit* unit);

void unit_draw(struct Unit* unit);
void unit_show(struct Unit* unit);
void unit_hide(struct Unit* unit);


#endif