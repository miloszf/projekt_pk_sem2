#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define MAX_ARGS_NUM 16

/** Struktura reprezentuj¹ca pojedyñczy takt rozkazu.
	Tworzy drzewiast¹ strukturê z jedn¹ lub dwoma ga³êziami. 
	Ostatnie ga³êzie (koniec instrucji) wskazuj¹ na 'NULL'. */
struct Tick
{
	struct Vector* signal_vect;	///< wektor sygna³ów danego taktu
	void* condition;			///< wskaŸnik na warunek
	struct Tick* next_if_true;	///< ga³¹Ÿ dla warunku logicznego '1'
	struct Tick* next;			///< ga³¹Ÿ dla warunku logicznego '0' lub jego braku (condition == NULL)
};

/** Funkcja inicjalizuj¹ca obiekt struktury Tick.
@return nowy obiekt */
struct Tick* tick_init();
/** Funkcja usuwaj¹ca podany obiekt
@param tick obiekt do usuniêcia */
void tick_delete(struct Tick* tick);

/** Struktura reprezentuj¹ca pojedyñczy rozkaz */
struct Instruction
{
	const char* name;			///< nazwa rozkazu
	unsigned char arguments;	///< liczba argumentów
	struct Tick* first_tick;	///< pierwszy takt
};

/** Funkcja inicjalizuj¹ca obiekt struktury Instruction.
@param name nazwa rozkazu
@return nowy obiekt */
struct Instruction* instruction_init(const char* name);
/** Funkcja usuwaj¹ca podany obiekt.
@param instruction obiekt do usuniêcia */
void instruction_delete(struct Instruction* instruction);

#endif