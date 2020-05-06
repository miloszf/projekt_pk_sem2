#include <stdlib.h>
#include <string.h>

#include "instruction.h"
#include "error.h"
#include "vector.h"

struct Tick* tick_init()
{
	struct Tick* new_tick = malloc_s(sizeof(struct Tick));
	*new_tick = (struct Tick){ vector_init(sizeof(struct Signal*)), NULL, NULL, NULL };
	return new_tick;
}

void tick_delete(struct Tick* tick)
{
	if (tick)
	{
		if (tick != tick->next)
			tick_delete(tick->next);
		if (tick != tick->next_if_true)
			tick_delete(tick->next_if_true);
		vector_delete(tick->signal_vect);
		free(tick);
	}
}

struct Instruction* instruction_init(const char* name)
{
	check_for_NULL(name);
	char* new_name = _strdup(name);
	if (!new_name)
		critical_error_set("strdup failed\n");
	struct Instruction* new_intr = malloc_s(sizeof(struct Instruction));
	//*new_intr = (struct Instruction){ new_name, 1, vector_init(sizeof(struct Tick*)) };
	*new_intr = (struct Instruction){ new_name, 1, NULL };
	return new_intr;
}

void instruction_delete(struct Instruction* instr)
{
	if (instr)
	{
		//struct Tick** tick_ptr;
		//while (tick_ptr = vector_pop(instr->tick_vect))
			//tick_delete(*tick_ptr);
		tick_delete(instr->first_tick);
		free((char*)instr->name);
		free(instr);
	}
}