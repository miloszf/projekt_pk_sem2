#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define MAX_ARGS_NUM 16

// Takt:
// Struktura reprezentuj�ca pojedy�czy takt wykonywany przez maszyn�W.
// Posiada tablic� sygna��w, wska�nik na warunek, wska�niki na nast�pny takt
// w zale�no�ci od logicznej warto�ci warunku (if-else)
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
// Struktura reprezentuj�ca pojedy�czy rozkaz wykonywany przez maszyn�W.
// Posiada nazw�, liczb� argument�w, wektor/tablic� takt�w.
struct Instruction
{
	const char* name;
	unsigned char arguments;
	//struct Vector* tick_vect;
	struct Tick* first_tick;
};

struct Instruction* instruction_init(const char* name);
void instruction_delete(struct Instruction* instruction);

// UWAGA: wska�nik na NULL jest traktowany jako wska�nik na czyt wys wei il
// (pierwszy takt ka�dego rozkazu)


#endif