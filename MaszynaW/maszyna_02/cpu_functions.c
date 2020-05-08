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
	var address = unit_read(memory_struct->reg_a);
	if (address == EMPTY)
		CRASH_LOG(LOG_UNKNOWN_VALUE);
		//critical_error_set("");
	var value = memory_struct->memory[address];
	return unit_immediate_set(memory_struct->reg_s, value) ? 0 : OUTPUT_ALREADY_SET;
	//if (unit_immediate_set(memory_struct->reg_s, value))
	//{
	//	unit_latch(memory_struct->reg_s);
	//	return 0;
	//}
	//else
	//	return OUTPUT_ALREADY_SET;
}

var sig_write_to_memory(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	struct SignalMemory* memory_struct = value_ptr;
	var address = unit_read(memory_struct->reg_a);
	var value = unit_read(memory_struct->reg_s);
	if (address == EMPTY || value == EMPTY)
		CRASH_LOG(LOG_UNKNOWN_VALUE);
		//critical_error_set("");
	memory_struct->memory[address] = value;
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
	var value = unit_read(instr_struct->from);
	if (value == EMPTY)
		return_value = EMPTY;
	else
	{
		value = (u_var)(value) >> *instr_struct->addr_len;
		if (value > *instr_struct->instr_num)
			value = EMPTY;
		if (!unit_set(instr_struct->to, value))
			return_value = OUTPUT_ALREADY_SET;
	}

	return return_value;
}

var sig_connect_buses(void* value_ptr)
{
	CHECK_IF_NULL(value_ptr);
	var return_value = 0;
	struct SignalBusConnection* bus_struct = value_ptr;
	var value = unit_read(bus_struct->from);
	if (value == EMPTY)
		return_value = EMPTY;
	else
	{
		value &= *bus_struct->mask;
		if (!unit_set(bus_struct->through, value) || !unit_set(bus_struct->to, value))
			return_value = OUTPUT_ALREADY_SET;
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