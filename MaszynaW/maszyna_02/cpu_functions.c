#include "cpu.h"
#include "cpu_functions.h"
#include "cpu_structs.h"
#include "error.h"
#include "vector.h"
#include "unit.h"
#include "signal.h"
#include "instruction.h"

var sig_read_from_memory(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	struct SignalMemory* memory_struct = value_ptr;
	CHECK_IF_NULL(memory_struct->memory);
	var address = unit_read(memory_struct->reg_a);
	if (address == EMPTY)
		CRASH_LOG(LOG_UNKNOWN_VALUE);
	var value = (*memory_struct->memory)[address];
	return unit_immediate_set(memory_struct->reg_s, value) ? 0 : OUTPUT_ALREADY_SET;
}

var sig_write_to_memory(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	struct SignalMemory* memory_struct = value_ptr;
	CHECK_IF_NULL(memory_struct->memory);
	var address = unit_read(memory_struct->reg_a);
	var value = unit_read(memory_struct->reg_s);
	if (address == EMPTY || value == EMPTY)
		CRASH_LOG(LOG_UNKNOWN_VALUE);
	(*memory_struct->memory)[address] = value;
	return 0;
}

var sig_stop(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	bool* stop = *(bool**)value_ptr;
	CHECK_IF_NULL(stop);
	*stop = true;
	return 0;
}

var sig_load_instruction(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	var return_value = 0;
	struct SignalInstruction* instr_struct = value_ptr;
	var word = unit_read(instr_struct->from);
	if (word == EMPTY)
		return_value = EMPTY;
	else
	{
		var code = (u_var)(word) >> *instr_struct->addr_len;
		if (code > *instr_struct->instr_num)
			word = EMPTY;
		if (!unit_set(instr_struct->to, word))
			return_value = OUTPUT_ALREADY_SET;
	}

	return return_value;
}

var sig_connect_buses(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	var return_value = 0;
	struct SignalBusConnection* bus_struct = value_ptr;
	var value_a = unit_read(bus_struct->bus_a);
	var value_b = unit_read(bus_struct->bus_b);
	if (value_a == ALREADY_SET || value_b == ALREADY_SET)
		return_value = ALREADY_SET;
	else
	{
		enum BusState { NONE_SET, BUS_A, BUS_B, BOTH_SET } bus_state = (value_a != EMPTY) + ((value_b != EMPTY) << 1);

		switch (bus_state)
		{
		case NONE_SET:
			return_value = EMPTY;
			break;
		case BUS_A:
		{
			value_a &= *bus_struct->mask;
			if (!unit_set(bus_struct->through, value_a) || !unit_set(bus_struct->bus_b, value_a))
				return_value = OUTPUT_ALREADY_SET;
		}
		break;
		case BUS_B:
		{
			value_b &= *bus_struct->mask;
			if (!unit_set(bus_struct->through, value_b) || !unit_set(bus_struct->bus_a, value_b))
				return_value = OUTPUT_ALREADY_SET;
		}
		break;
		case BOTH_SET:
			return_value = OUTPUT_ALREADY_SET;
			break;
		default:
			CRASH_LOG(LOG_UNKNOWN_VALUE);
			break;
		}
	}

	return return_value;
}

#define INPUT 1
#define OUTPUT 2

var sig_io_handling(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	struct SignalIOHandling* io_struct = value_ptr;
	var address = unit_read(io_struct->address_reg);
	
	if (address == EMPTY)
		CRASH_LOG(LOG_UNKNOWN_VALUE);
	else
	{
		address &= *io_struct->addr_mask;
		switch (address)
		{
		case INPUT:
		{
			*io_struct->input_flag = true;
			if (*io_struct->output_flag)
				*io_struct->output_flag = false;
		}
		break;
		case OUTPUT:
		{
			*io_struct->output_flag = true;
			if (*io_struct->input_flag)
				*io_struct->input_flag = false;
		}
		break;
		default:
			runtime_error_set(INVALID_IO_ADDRESS, NULL);
		}
	}

	return 0;
}

var sig_enable_interrupts(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	var return_value = 0;
	struct SignalInterrupts* intr_struct = value_ptr;
	var interrupts = unit_read(intr_struct->reg_rz);
	var interrupts_mask = unit_read(intr_struct->reg_rm);
	if (interrupts == EMPTY || interrupts_mask == EMPTY)
		CRASH_LOG(LOG_UNKNOWN_VALUE);
	if (interrupts &= ~interrupts_mask)
	{
		var highest_priority_interrupt = 0;
		var interrupt_address = 1;
		for (; interrupt_address <= CPU_INTERRUPTS_NUMBER; interrupt_address++)
		{
			if (highest_priority_interrupt = (interrupts & (1 << (CPU_INTERRUPTS_NUMBER - interrupt_address))))
				break;
		}

		if (!unit_set(intr_struct->reg_rp, highest_priority_interrupt) || !unit_set(intr_struct->reg_ap, interrupt_address))
			return_value = ALREADY_SET;
		else
			*intr_struct->int_tag = true;		
	}
	else
		*intr_struct->int_tag = false;

	return return_value;
}

var sig_reset_interrupts(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	var return_value = 0;
	struct SignalInterrupts* intr_struct = value_ptr;
	var interrupts = unit_read(intr_struct->reg_rz);
	var current_interrupt = unit_read(intr_struct->reg_rp);
	if (interrupts == EMPTY || current_interrupt == EMPTY)
		CRASH_LOG(LOG_UNKNOWN_VALUE);
	if (current_interrupt)
	{
		interrupts &= ~current_interrupt;
		if (!unit_set(intr_struct->reg_rz, interrupts) || !unit_set(intr_struct->reg_rp, 0) || !unit_set(intr_struct->reg_ap, 0))
			return_value = ALREADY_SET;
	}

	return return_value;
}

var sig_pass(var value, var mask)
{
	return (value & mask);
}

var sig_increment(var value, var mask)
{
	return (++value & mask);
}

var sig_decrement(var value, var mask)
{
	return (--value & mask);
}

var sig_shift_right(var value, var mask)
{
	return (((u_var)(value) >> 1) & mask);
}

var sig_bitwise_complement(var value, var mask)
{
	return ((~value) & mask);
}

var sig_add(var value_a, var value_b, var mask)
{
	return ((value_a + value_b) & mask);
}

var sig_substract(var value_a, var value_b, var mask)
{
	return ((value_a - value_b) & mask);
}

var sig_multiply(var value_a, var value_b, var mask)
{
	return ((value_a * value_b) & mask);
}

var sig_divide(var value_a, var value_b, var mask)
{
	return ((value_a / value_b) & mask);
}

var sig_bitwise_or(var value_a, var value_b, var mask)
{
	return (value_a | value_b);
}

var sig_bitwise_and(var value_a, var value_b, var mask)
{
	return (value_a & value_b);
}