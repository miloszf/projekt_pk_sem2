#include <stdlib.h>
#include <string.h>

#include "cpu_structs.h"
#include "error.h"
#include "vector.h"
#include "unit.h"
//#include "signal.h"
#include "graphics.h"
#include "instruction.h"

bool cpu_tag_update(struct CPUTag* tag, struct CPU* cpu)
{
	CHECK_IF_NULL(tag);
	CHECK_IF_NULL(cpu);
	bool return_value;

	switch (tag->type)
	{
	case TAG_Z:
	{
		var value = unit_read(cpu->components.alu.reg_ak);
		if (value != EMPTY)
			return_value = value & (cpu->word.word_mask - (cpu->word.word_mask >> 1));
		else
			return_value = false;
	}
	break;
	case TAG_ZAK:
	{
		return_value = !unit_read(cpu->components.alu.reg_ak);
	}
	break;
	case TAG_V:
	{
		return_value = false;
	}
	break;
	case TAG_INT:
	{
		return_value = tag->value;
	}
	break;
	default:
		//critical_error_set("");
		CRASH_LOG(LOG_UNKNOWN_VALUE);
		return_value = false;
	}

	return return_value;
}

struct CPUTag cpu_tag_init(const char* name, CPUTagType type)
{
	CHECK_IF_NULL(name);
	char* new_name = _strdup(name);
	if (!new_name)
		CRASH_LOG(LIBRARY_FUNC_FAILURE);
		//critical_error_set("strdup failed\n");
	return (struct CPUTag) { new_name, false, type };
}

void cpu_tag_delete(struct CPUTag* tag)
{
	if (tag)
	{
		free((char*)tag->name);
	}
}

struct CPUMemory* cpu_memory_init(struct Canvas* canvas, const Point position, const var* addr_length)
{
	struct Vector* instr_names_vect = vector_init(sizeof(const char*));
	struct CPUMemory* memory = malloc_s(sizeof(struct CPUMemory));
	*memory = (struct CPUMemory){ NULL, addr_length, NULL, instr_names_vect };;
	struct DrawableMemoryInit init =
	{
		canvas,
		position,
		&memory->memory_array,
		addr_length,
		&memory->instr_names_vect
	};
	memory->drawable = drawable_new_memory(&init);
	return memory;
}

void cpu_memory_update(struct CPUMemory* memory, struct Vector* instr_vect)
{
	CHECK_IF_NULL(memory);
	CHECK_IF_NULL(memory->addr_len);

	if (memory->memory_array)
		free(memory->memory_array);
	unsigned mem_size = 1 << (*(memory->addr_len));
	memory->memory_array = calloc_s(mem_size, sizeof(var));

	const char** name_ptr;
	while (name_ptr = vector_pop(memory->instr_names_vect))
	{
		free((char*)*name_ptr);
		free(name_ptr);
	}

	size_t vect_size = vector_size(instr_vect);
	for (unsigned i = 0; i < vect_size; i++)
	{
		struct Instruction** instr_ptr = vector_read(instr_vect, i);
		CHECK_IF_NULL(instr_ptr);
		CHECK_IF_NULL(*instr_ptr);
		char* name = _strdup((*instr_ptr)->name);
		vector_push(memory->instr_names_vect, &name);
	}
}

void cpu_memory_scroll(struct CPUMemory* memory, var offset)
{
	CHECK_IF_NULL(memory);
	drawable_set_value(memory->drawable, &offset);
}

void cpu_memory_delete(struct CPUMemory* memory)
{
	if (memory)
	{
		const char** name_ptr;
		while (name_ptr = vector_pop(memory->instr_names_vect))
		{
			free((char*)*name_ptr);
			free(name_ptr);
		}
			
		vector_delete(memory->instr_names_vect);
		free(memory->memory_array);
		free(memory);
	}
}

struct CPUWord cpu_word_init()
{	
	struct CPUWord word = {
		.char_mask = 0x7F,
		.intr_mask = 0x0F
	};
	cpu_word_update(&word, 3, 5, NULL);
	return word;
}

void  cpu_word_update(struct CPUWord* word, var code_length, var address_length, struct Vector* instr_vect)
{
	CHECK_IF_NULL(word);
	word->addr_len = address_length;
	word->word_len = word->addr_len + code_length;
	word->word_mask = (u_var)(-1) >> (sizeof(var) * 8 - word->word_len);
	word->addr_mask = (u_var)(word->word_mask) >> (code_length);
	word->instr_num = instr_vect ? vector_size(instr_vect) : 0;
}

void cpu_peripherals_update_buttons(var interrupts, struct Drawable** buttons_array)
{
	CHECK_IF_NULL(buttons_array);
	for (int i = 1; i <= CPU_INTERRUPTS_NUMBER; i++)
	{
		bool is_button_set = interrupts & (1 << (CPU_INTERRUPTS_NUMBER - i));
		drawable_set_value(buttons_array[i - 1], &is_button_set);
	}
}