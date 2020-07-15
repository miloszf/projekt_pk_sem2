#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "cpu.h"
#include "cpu_structs.h"
#include "cpu_components.h"
#include "cpu_setup.h"
#include "cpu_functions.h"
#include "vector.h"
#include "error.h"
#include "settings.h"
#include "unit.h"
#include "signal.h"
#include "graphics.h"
#include "map.h"
#include "file.h"
#include "instruction.h"
#include "color.h"

struct CPU* cpu_init(struct Canvas* canvas)
{
	CHECK_IF_NULL(canvas);

	struct CPU* cpu = malloc_s(sizeof(struct CPU));
	cpu->setup = cpu_setup_init();
	cpu->vector.units = NULL;
	cpu->vector.signals = NULL;
	cpu->vector.instructions = NULL;
	cpu->word = cpu_word_init();
	cpu->memory = cpu_memory_init(canvas, POINT(48, 12), &cpu->word.addr_len);
	cpu->runtime = (struct CPURuntime){ NULL, NULL, false };
	cpu->peripherals = (struct CPUPeripherals){ false, false };
	struct ColorSet buttons_color_set = {
		.f_default = 0,
		.b_default = BACKGROUND_INTENSITY,
		.f_active = 0,
		.b_active = (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
	};
	for (int i = 0; i < CPU_INTERRUPTS_NUMBER; i++)
	{
		char text[4];
		int chars_written = sprintf_s(text, 4, " %1d ", i + 1);
		cpu->peripherals.buttons_array[i] = drawable_new_button(canvas, POINT(2 + 4 * i, 1), POINT(3, 1), text, &buttons_color_set);
	}
		
	drawable_new_frame(canvas, POINT(0, 0), POINT(75, 31));

	cpu_init_alu_units(cpu, POINT(21, 13), canvas);
	cpu_init_mem_units(cpu, POINT(48, 8), canvas);
	cpu_init_addr_units(cpu, POINT(1, 8), canvas);
	cpu_init_xy_units(cpu, POINT(1, 25), canvas);
	cpu_init_stack_units(cpu, POINT(22, 8), canvas);
	cpu_init_io_units(cpu, POINT(37, 25), canvas);
	cpu_init_intr_units(cpu, POINT(1, 0), canvas);

	cpu_init_alu_signals(cpu, POINT(21, 13), canvas);
	cpu_init_mem_signals(cpu, POINT(48, 8), canvas);
	cpu_init_addr_signals(cpu, POINT(1, 8), canvas);
	cpu_init_xy_signals(cpu, POINT(1, 25), canvas);
	cpu_init_stack_signals(cpu, POINT(22, 8), canvas);
	cpu_init_io_signals(cpu, POINT(37, 25), canvas);
	cpu_init_intr_signals(cpu, POINT(1, 0), canvas);

	cpu->components.tags.all.tag_int = cpu_tag_init("int", TAG_INT);
	cpu->components.tags.all.tag_zak = cpu_tag_init("zak", TAG_ZAK);
	cpu->components.tags.all.tag_z = cpu_tag_init("z", TAG_Z);
	cpu->components.tags.all.tag_v = cpu_tag_init("v", TAG_V);

	return cpu;
}

bool cpu_import_instructions(struct CPU* cpu, const char* file_name)
{
	CHECK_IF_NULL(cpu);

	struct FileHandler* files_handler = file_handler_init();
	struct Map* setup_map = map_init(sizeof(&cpu->setup.list->value));
	size_t setup_size = sizeof(cpu->setup.list) / sizeof(*cpu->setup.list);
	for (unsigned i = 0; i < setup_size; i++)
	{
		const char* key = cpu->setup.list[i].name;
		void* value_ptr = &cpu->setup.list[i].value;
		map_push(setup_map, key, &value_ptr);
	}

	if (file_import_setup(file_name, files_handler, setup_map))
	{
		if (cpu->vector.units)
			vector_delete(cpu->vector.units);
		if (cpu->vector.signals)
			vector_delete(cpu->vector.signals);

		cpu->vector.units = vector_init(sizeof(struct Unit*));
		cpu->vector.signals = vector_init(sizeof(struct Signal*));
		struct Map* signal_map = map_init(sizeof(struct Signal*));
		drawable_set_visibility(cpu->memory->drawable, true);

		for (int i = 0; i < CPU_SETUP_SIZE; i++)
		{
			bool is_set = cpu->setup.list[i].value;
			int unit_vect_size = vector_size(cpu->setup.list[i].unit_vect);
			for (int index = 0; index < unit_vect_size; index++)
			{
				struct Unit** unit_ptr = vector_read(cpu->setup.list[i].unit_vect, index);
				CHECK_IF_NULL(unit_ptr);
				unit_set_visibility(*unit_ptr, is_set);
				vector_push(cpu->vector.units, unit_ptr);
			}
			int signal_vect_size = vector_size(cpu->setup.list[i].signal_vect);
			for (int index = 0; index < signal_vect_size; index++)
			{
				struct Signal** signal_ptr = vector_read(cpu->setup.list[i].signal_vect, index);
				CHECK_IF_NULL(signal_ptr);
				signal_set_visibility(*signal_ptr, is_set);
				vector_push(cpu->vector.signals, signal_ptr);
				if (is_set)
				{
					const char* key = signal_get_name(*signal_ptr);
					map_push(signal_map, key, signal_ptr);
				}
			}
		}

		for (int i = 0; i < CPU_INTERRUPTS_NUMBER; i++)
			drawable_set_visibility(cpu->peripherals.buttons_array[i], cpu->setup.all.interrupts.value);

		struct Map* tag_map = map_init(sizeof(struct CPUTag));
		for (int i = 0; i < CPU_TAGS_NUMBER; i++)
		{
			const char* key = cpu->components.tags.list[i].name;
			void* value_ptr = cpu->components.tags.list + i;
			map_push(tag_map, key, &value_ptr);
		}

		if (cpu->vector.instructions)
		{
			struct Instruction** instr_ptr;
			while (instr_ptr = vector_pop(cpu->vector.instructions))
			{
				instruction_delete(*instr_ptr);
				free(instr_ptr);
			}
			vector_delete(cpu->vector.instructions);
		}
			
		cpu->vector.instructions = file_compile_instructions(files_handler, signal_map, tag_map);

		map_delete(signal_map);
		map_delete(tag_map);
	}

	map_delete(setup_map);
	file_handler_delete(files_handler);

	if (!error())
	{
		cpu_word_update(&cpu->word, cpu->setup.all.code_length.value, cpu->setup.all.addr_length.value, cpu->vector.instructions);
		cpu_memory_update(cpu->memory, cpu->vector.instructions);
	}

	return !error();
}

bool cpu_import_program(struct CPU* cpu, const char* file_name)
{
	CHECK_IF_NULL(cpu);
	var code = cpu->word.word_len - cpu->word.addr_len;
	bool result = file_compile_program(file_name, cpu->vector.instructions, cpu->word.addr_len, code, cpu->memory->memory_array);
	if (result)
		cpu_memory_scroll(cpu->memory, 0);
	return result;
}

void* cpu_tick(struct CPU* cpu)
{
	CHECK_IF_NULL(cpu);
	if (cpu->runtime.stop)
	{
		runtime_error_set(CPU_STOPPED, NULL);
		return false;
	}

	// reset all
	{
		unsigned units_num = vector_size(cpu->vector.units);
		for (unsigned i = 0; i < units_num; i++)
		{
			struct Unit** unit_ptr = vector_read(cpu->vector.units, i);
			CHECK_IF_NULL(unit_ptr);
			unit_reset(*unit_ptr);
		}
		unsigned signals_num = vector_size(cpu->vector.signals);
		for (unsigned i = 0; i < signals_num; i++)
		{
			struct Signal** signal_ptr = vector_read(cpu->vector.signals, i);
			CHECK_IF_NULL(signal_ptr);
			signal_reset(*signal_ptr);
		}
	}

	var value = unit_read(cpu->components.addr.reg_i);
	if (value == EMPTY)
	{
		runtime_error_set(UNKNOWN_INSTRUCTION, NULL);
		cpu->runtime = (struct CPURuntime){ NULL, NULL, true };
	}

	if (!error())
	{
		var instr_code = (u_var)value >> cpu->word.addr_len;
		struct Instruction** instr_ptr = vector_read(cpu->vector.instructions, instr_code);
		CHECK_IF_NULL(instr_ptr);
		CHECK_IF_NULL(*instr_ptr);

		if (!cpu->runtime.current_tick)
		{
			cpu->runtime.current_instr = *instr_ptr;
			cpu->runtime.current_tick = (*instr_ptr)->first_tick;
		}
		else if (*instr_ptr != cpu->runtime.current_instr)
		{
			cpu->runtime.current_instr = *instr_ptr;
			cpu->runtime.current_tick = (*instr_ptr)->first_tick->next;
		}

		CHECK_IF_NULL(cpu->runtime.current_tick);
		size_t signal_ptr_array_size;
		struct Signal** signal_ptr_array = vector_convert_to_array(vector_copy(cpu->runtime.current_tick->signal_vect), &signal_ptr_array_size);
		size_t current_array_size = 0;
		size_t new_array_size = signal_ptr_array_size;
		while (!error() && new_array_size && current_array_size != new_array_size)
		{
			current_array_size = new_array_size;
			for (size_t i = 0; i < signal_ptr_array_size && !error(); i++)
				if (signal_ptr_array[i])
				{
					var result = signal_set(signal_ptr_array[i]);
					if (result == ALREADY_SET)
						runtime_error_set(ALREADY_SET, signal_get_name(signal_ptr_array[i]));
					else if (!result)
					{
						signal_ptr_array[i] = NULL;
						new_array_size--;
					}
				}
		}

		if (new_array_size && signal_ptr_array_size)
			runtime_error_set(EMPTY_UNIT, NULL);
		free(signal_ptr_array);
	}

	// latch regs
	{
		unsigned units_num = vector_size(cpu->vector.units);
		for (unsigned i = 0; i < units_num; i++)
		{
			struct Unit** unit_ptr = vector_read(cpu->vector.units, i);
			CHECK_IF_NULL(unit_ptr);
			unit_latch(*unit_ptr);
		}
	}

	if (!error())
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

	return error() ? NULL : cpu->runtime.current_tick;
}

void cpu_get_io_flags(struct CPU* cpu, bool* input_flag, bool* output_flag)
{
	CHECK_IF_NULL(cpu);
	CHECK_IF_NULL(input_flag);
	CHECK_IF_NULL(output_flag);
	*input_flag = cpu->peripherals.input_flag;
	*output_flag = cpu->peripherals.output_flag;
}

void cpu_user_input(struct CPU* cpu, var mouse_scroll, var interrupts, char input_char, char* output_char)
{
	CHECK_IF_NULL(cpu);
	drawable_set_value(cpu->memory->drawable, &mouse_scroll);

	if (cpu->setup.all.interrupts.value)
	{
		var old_interrupts = unit_read(cpu->components.intr.reg_rz);
		if (old_interrupts == EMPTY)
			CRASH_LOG(LOG_UNKNOWN_VALUE);
		interrupts = (interrupts | old_interrupts) & cpu->word.intr_mask;
		if (interrupts != old_interrupts)
		{
			unit_immediate_set(cpu->components.intr.reg_rz, interrupts & cpu->word.intr_mask);
			unit_reset(cpu->components.intr.reg_rz);
		}
		if (interrupts != cpu->peripherals.buttons_set)
		{
			cpu_peripherals_update_buttons(interrupts, cpu->peripherals.buttons_array);
			cpu->peripherals.buttons_set = interrupts;
		}
	}

	enum IOState { READY, INPUT, OUTPUT} io_state = cpu->peripherals.input_flag + (cpu->peripherals.output_flag << 1);
	const char CHAR_MASK = 0x7F;
	const var IO_READY = 1;

	switch (io_state)
	{
	case READY:
		break;
	case INPUT:
	{
		if (input_char)
		{
			unit_immediate_set(cpu->components.io.reg_rb, input_char);
			unit_immediate_set(cpu->components.io.reg_g, IO_READY);
			cpu->peripherals.input_flag = false;
		}
		else 
			unit_immediate_set(cpu->components.io.reg_g, !IO_READY);
	}
	break;
	case OUTPUT:
	{
		*output_char = unit_read(cpu->components.io.reg_rb);
		unit_immediate_set(cpu->components.io.reg_g, IO_READY);
		cpu->peripherals.output_flag = false;
	}
	break;
	default:
		CRASH_LOG(LOG_UNKNOWN_VALUE);
	}
}

void cpu_reset(struct CPU* cpu)
{
	CHECK_IF_NULL(cpu);
	int unit_vect_size = vector_size(cpu->vector.units);
	for (int index = 0; index < unit_vect_size; index++)
	{
		struct Unit** unit_ptr = vector_read(cpu->vector.units, index);
		CHECK_IF_NULL(unit_ptr);
		unit_restart(*unit_ptr);
	}
	int signal_vect_size = vector_size(cpu->vector.signals);
	for (int index = 0; index < signal_vect_size; index++)
	{
		struct Signal** signal_ptr = vector_read(cpu->vector.signals, index);
		CHECK_IF_NULL(signal_ptr);
		signal_reset(*signal_ptr);
	}
	cpu->runtime.current_instr = NULL;
	cpu->runtime.current_tick = NULL;
	cpu->runtime.stop = false;
	for (int index = 0; index < CPU_TAGS_NUMBER; index++)
		cpu->components.tags.list[index].value = false;
	cpu->peripherals.input_flag = false;
	cpu->peripherals.output_flag = false;
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
		if (cpu->vector.instructions)
		{
			struct Instruction** instr_ptr;
			while (instr_ptr = vector_pop(cpu->vector.instructions))
			{
				instruction_delete(*instr_ptr);
				free(instr_ptr);
			}
		}
		vector_delete(cpu->vector.instructions);
		cpu->vector.instructions = NULL;
		// delete units & signals
		vector_delete(cpu->vector.signals);
		vector_delete(cpu->vector.units);
		// delete memory
		cpu_memory_delete(cpu->memory);

		free(cpu);
	}
}