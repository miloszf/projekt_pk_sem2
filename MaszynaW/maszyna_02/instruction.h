#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define MAX_ARGS_NUM 16

/** Struktura reprezentuj�ca pojedy�czy takt rozkazu.
	Tworzy drzewiast� struktur� z jedn� lub dwoma ga��ziami. 
	Ostatnie ga��zie (koniec instrucji) wskazuj� na 'NULL'. */
struct Tick
{
	struct Vector* signal_vect;	///< wektor sygna��w danego taktu
	void* condition;			///< wska�nik na warunek
	struct Tick* next_if_true;	///< ga��� dla warunku logicznego '1'
	struct Tick* next;			///< ga��� dla warunku logicznego '0' lub jego braku (condition == NULL)
};

/** Funkcja inicjalizuj�ca obiekt struktury Tick.
@return nowy obiekt */
struct Tick* tick_init();
/** Funkcja usuwaj�ca podany obiekt
@param tick obiekt do usuni�cia */
void tick_delete(struct Tick* tick);

/** Struktura reprezentuj�ca pojedy�czy rozkaz */
struct Instruction
{
	const char* name;			///< nazwa rozkazu
	unsigned char arguments;	///< liczba argument�w
	struct Tick* first_tick;	///< wska�nik na pierwszy takt
};

/** Funkcja inicjalizuj�ca obiekt struktury Instruction.
@param name nazwa rozkazu
@return nowy obiekt */
struct Instruction* instruction_init(const char* name);
/** Funkcja usuwaj�ca podany obiekt
@param instruction obiekt do usuni�cia */
void instruction_delete(struct Instruction* instruction);

#endif