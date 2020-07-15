#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define MAX_ARGS_NUM 16

// Takt:
// Struktura reprezentuj¹ca pojedyñczy takt wykonywany przez maszynêW.
// Posiada tablicê sygna³ów, wskaŸnik na warunek, wskaŸniki na nastêpny takt
// w zale¿noœci od logicznej wartoœci warunku (if-else)
struct Tick
{
	struct Vector* signal_vect;
	void* condition;
	struct Tick* next_if_true;
	struct Tick* next;
};

struct Tick* tick_init();
void tick_delete(struct Tick* tick);

// Instruckja:
// Struktura reprezentuj¹ca pojedyñczy rozkaz wykonywany przez maszynêW.
// Posiada nazwê, liczbê argumentów, wektor/tablicê taktów.
struct Instruction
{
	const char* name;
	unsigned char arguments;
	struct Tick* first_tick;
};

struct Instruction* instruction_init(const char* name);
void instruction_delete(struct Instruction* instruction);

#endif