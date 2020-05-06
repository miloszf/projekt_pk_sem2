#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// DEBUG
#include <stdio.h>

#include "cpu_structs.h"
#include "cpu_components.h"
#include "cpu_setup.h"
#include "vector.h"
#include "error.h"
#include "settings.h"
#include "unit.h"
#include "signal.h"
#include "graphics.h"
#include "map.h"
#include "file.h"
#include "instruction.h"



bool cpu_import_instructions(struct CPU* cpu)
{
	check_for_NULL(cpu);

	Error error = NO_ERROR;
	struct FileHandler* files_handler = file_handler_init();

	struct Map* setup_map = map_init(sizeof(&cpu->setup.list->value));
	size_t setup_size = sizeof(cpu->setup.list) / sizeof(*cpu->setup.list);
	for (unsigned i = 0; i < setup_size; i++)
	{
		const char* key = cpu->setup.list[i].name;
		void* value_ptr = &cpu->setup.list[i].value;
		map_push(setup_map, key, &value_ptr);
	}
	
	if (file_import_setup("MaszynaW.lst", files_handler, setup_map))
	{
		if (cpu->vector.units)
			vector_delete(cpu->vector.units);
		if (cpu->vector.signals)
			vector_delete(cpu->vector.signals);

		cpu->vector.units = vector_init(sizeof(struct Unit*));
		cpu->vector.signals = vector_init(sizeof(struct Signal*));
		struct Map* signal_map = map_init(sizeof(struct Signal*));

		for (int i = 0; i < CPU_SETUP_SIZE; i++)
		{
			bool is_set = cpu->setup.list[i].value;
			int unit_vect_size = vector_size(cpu->setup.list[i].unit_vect);
			for (int index = 0; index < unit_vect_size; index++)
			{
				struct Unit** unit_ptr = vector_read(cpu->setup.list[i].unit_vect, index);
				check_for_NULL(unit_ptr);
				unit_set_visibility(*unit_ptr, is_set);
				vector_push(cpu->vector.units, unit_ptr);
			}
			int signal_vect_size = vector_size(cpu->setup.list[i].signal_vect);
			for (int index = 0; index < signal_vect_size; index++)
			{
				struct Signal** signal_ptr = vector_read(cpu->setup.list[i].signal_vect, index);
				check_for_NULL(signal_ptr);
				signal_set_visibility(*signal_ptr, is_set);
				vector_push(cpu->vector.signals, signal_ptr);
				if (is_set)
				{
					const char* key = signal_get_name(*signal_ptr);
					map_push(signal_map, key, signal_ptr);
				}
			}
		}

		struct Map* tag_map = map_init(sizeof(struct CPUTag));
		for (int i = 0; i < CPU_TAGS_NUMBER; i++)
		{
			const char* key = cpu->components.tags.list[i].name;
			void* value_ptr = cpu->components.tags.list + i;
			map_push(tag_map, key, &value_ptr);
		}

		//struct Vector* instr_vect = file_compile_instructions(files_handler, signal_map, tag_map);
		cpu->vector.instructions = file_compile_instructions(files_handler, signal_map, tag_map);
		if (!cpu->vector.instructions)
			error = ERROR;

		map_delete(signal_map);
		map_delete(tag_map);
	}
	else
		error = ERROR;

	map_delete(setup_map);
	file_handler_delete(files_handler);
	return !error;
}

bool cpu_import_program(struct CPU* cpu)
{
	check_for_NULL(cpu);

	var addr = cpu->setup.all.addr_length.value;
	var word = addr + cpu->setup.all.code_length.value;
	var word_mask = (u_var)(-1) >> (sizeof(var) * 8 - word);
	var addr_mask = (u_var)word_mask >> (cpu->setup.all.code_length.value);
	var char_mask = 0x7F;
	var intr_mask = 0x0F;
	var instr_num = vector_size(cpu->vector.instructions);
	cpu->word = (struct CPUWord){ word, addr, word_mask, addr_mask, char_mask, instr_num };

	if (cpu->memory)
		free(cpu->memory);
	unsigned mem_size = 1 << (addr);
	cpu->memory = calloc_s(1 << (addr), sizeof(var));

	file_compile_program("program.prg", cpu->vector.instructions, addr, word - addr, cpu->memory);

	return true;
}

struct CPU* cpu_init(struct Canvas* canvas)
{
	check_for_NULL(canvas);

	struct CPU* cpu = malloc_s(sizeof(struct CPU));
	cpu->setup = cpu_setup_init();
	cpu->vector.units = NULL;
	cpu->vector.signals = NULL;
	cpu->vector.instructions = NULL;
	cpu->memory = NULL;
	cpu->runtime = (struct CPURuntime){ NULL, NULL, false };

	cpu_init_alu_units(cpu, POINT(20, 13), canvas);
	cpu_init_mem_units(cpu, POINT(47, 8), canvas);
	cpu_init_addr_units(cpu, POINT(1, 8), canvas);
	cpu_init_xy_units(cpu, POINT(1, 25), canvas);
	cpu_init_stack_units(cpu, POINT(22, 8), canvas);
	cpu_init_io_units(cpu, POINT(37, 25), canvas);
	cpu_init_intr_units(cpu, POINT(1, 0), canvas);

	cpu_init_alu_signals(cpu, POINT(20, 13), canvas);
	cpu_init_mem_signals(cpu, POINT(47, 8), canvas);
	cpu_init_addr_signals(cpu, POINT(1, 8), canvas);
	cpu_init_xy_signals(cpu, POINT(1, 25), canvas);
	cpu_init_stack_signals(cpu, POINT(22, 8), canvas);
	cpu_init_io_signals(cpu, POINT(37, 25), canvas);
	cpu_init_intr_signals(cpu, POINT(1, 0), canvas);

	cpu->components.tags.all.tag_int = cpu_tag_init("int", TAG_INT);
	cpu->components.tags.all.tag_zak = cpu_tag_init("zak", TAG_ZAK);
	cpu->components.tags.all.tag_z = cpu_tag_init("z", TAG_Z);
	cpu->components.tags.all.tag_v = cpu_tag_init("v", TAG_V);

	// doda� obs�ug� b��d�w
	Error error = cpu_import_instructions(cpu) ? NO_ERROR : ERROR;
	cpu_import_program(cpu);

	return cpu;
}

void* cpu_tick(struct CPU* cpu)
{
	check_for_NULL(cpu);
	if (cpu->runtime.stop)
	{
		error_set(ERROR_CPU_STOPPED);
		return false;
	}

	Error error = NO_ERROR;
	var value = unit_read(cpu->components.addr.reg_i);
	if (value == EMPTY)
	{
		error = ERROR_INSTR_ADDR_OUT_OF_RANGE;
		cpu->runtime = (struct CPURuntime){ NULL, NULL, true };
	}

	if (!error)
	{
		var address = (u_var)(value) >> (cpu->word.word_len - cpu->word.addr_len);
		struct Instruction** instr_ptr = vector_read(cpu->vector.instructions, address);
		check_for_NULL(instr_ptr);
		check_for_NULL(*instr_ptr);

		if (!cpu->runtime.current_tick  || *instr_ptr != cpu->runtime.current_instr)
		{
			cpu->runtime.current_instr = *instr_ptr;
			cpu->runtime.current_tick = (*instr_ptr)->first_tick;
		}

		check_for_NULL(cpu->runtime.current_tick);
		size_t signal_ptr_array_size;
		struct Signal** signal_ptr_array = vector_unwrap(vector_copy(cpu->runtime.current_tick->signal_vect), &signal_ptr_array_size);
		size_t current_array_size = 0;
		size_t new_array_size = signal_ptr_array_size;
		while (!error && current_array_size != new_array_size)
		{
			current_array_size = new_array_size;
			for (size_t i = 0; i < signal_ptr_array_size && !error; i++)
			{
				if (signal_ptr_array[i])
				{
					var result = signal_set(signal_ptr_array[i]);
					if (result == OUTPUT_ALREADY_SET)
						error = ERROR_UNIT_ALREADY_SET;
					else if (!result)
					{
						signal_ptr_array[i] = NULL;
						new_array_size--;
					}
				}

			}
		}

		if (!current_array_size)
			error = ERROR_EMPTY_UNIT;
		free(signal_ptr_array);
	}

	if (!error)
	{
		if (cpu->runtime.current_tick->condition)
		{
			struct CPUTag* tag = cpu->runtime.current_tick->condition;
			if (cpu_tag_update(tag, cpu))
				cpu->runtime.current_tick = cpu->runtime.current_tick->next_if_true;
			else
				cpu->runtime.current_tick = cpu->runtime.current_tick->next;
		}
		else
			cpu->runtime.current_tick = cpu->runtime.current_tick->next;
	}
	else
		error_set(error);

	return error ? NULL : cpu->runtime.current_tick;
}

void cpu_delete(struct CPU* cpu)
{
	if (cpu)
	{
		// delete setup 
		cpu_setup_delete(&cpu->setup);
		// delete tags
		for (int i = 0; i < CPU_TAGS_NUMBER; i++)
			cpu_tag_delete(&cpu->components.tags.list[i]);
		// delete instructions
		struct Instruction** instr_ptr;
		while (instr_ptr = vector_pop(cpu->vector.instructions))
			instruction_delete(*instr_ptr);
		vector_delete(cpu->vector.instructions);
		cpu->vector.instructions = NULL;
		// delete units & signals
		vector_delete(cpu->vector.signals);
		vector_delete(cpu->vector.units);
		// delete memory
		free(cpu->memory);

		free(cpu);
	}
}