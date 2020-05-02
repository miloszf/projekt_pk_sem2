#ifndef CPU_FUNCTIONS_H
#define CPU_FUNCTIONS_H

#include <stdbool.h>
#include "settings.h"

struct SignalMemory
{
	var* memory;
	struct Unit* reg_a;
	struct Unit* reg_s;
};

struct SignalInterrupts
{
	struct Unit* reg_rz;
	struct Unit* reg_rm;
	struct Unit* reg_rp;
	struct Unit* reg_ap;
	bool* int_tag;
};

struct SignalInstruction
{
	struct Unit* from;
	struct Unit* to;
	const var* addr_len;
	const var* instr_num;
};

struct SignalBusConnection
{
	struct Unit* from;
	struct Unit* through;
	struct Unit* to;
	const var* mask;
};

var sig_read_from_memory(void*);
var sig_write_to_memory(void*);
//var sig_enable_interrupts(void*);
//var sig_reset_interrupts(void*);
var sig_stop(void*);
var sig_load_instruction(void*);
var sig_connect_buses(void*);
// IO?

var sig_pass(var value, var mask);
var sig_increment(var value, var mask);
var sig_decrement(var value, var mask);
var sig_shift_right(var value, var mask);
var sig_bitwise_complement(var value, var mask);

var sig_add(var value_a, var value_b, var mask);
var sig_substract(var value_a, var value_b, var mask);
var sig_multiply(var value_a, var value_b, var mask);
var sig_divide(var value_a, var value_b, var mask);
var sig_bitwise_or(var value_a, var value_b, var mask);
var sig_bitwise_and(var value_a, var value_b, var mask);


#endif