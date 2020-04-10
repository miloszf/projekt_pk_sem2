#include <stdlib.h>

#include "reg.h"

#define EMPTY (var)-1

struct Reg
{
	var value;
	struct Reg* input;
	//struct Reg** outputs;
	RegFun set;
	RegFun reset;
	union {
		var to_set;
		struct {
			struct Reg* input_ptr;
			var(*get_value)(var, var);
		} comb;
	};
	
};

struct Reg* reg_init(RegFun set, RegFun reset, var(*get_value)(var, var), struct Reg* input_ptr)
{
	struct Reg* reg = malloc(sizeof(struct Reg));

	if (reg)
	{
		reg->value = EMPTY;
		reg->input = NULL;
		//->outputs = NULL;
		reg->set = set;
		reg->reset = reset;
		if (input_ptr)
		{
			reg->comb.input_ptr = input_ptr;
			reg->comb.get_value = get_value;
		}
		else
			reg->to_set = EMPTY;
	}

	return reg;
}

struct Reg* reg_init(RegFun set, RegFun reset, var(*get_value)(var, var))
{
	return reg_init(set, reset, get_value, NULL);
}

struct Reg* reg_init(RegFun set, RegFun reset)
{
	return reg_init(set, reset, NULL, NULL);
}

Error comb_set(struct Reg* reg)
{
	Error error;

	if (!reg)
		error = ERROR_NULL_POINTER;
	else if (!reg->input || reg->input->value == EMPTY)
		error = WARNING_NO_INPUT;
	else 
	{
		var value2 = (reg->comb.input_ptr) ? reg->comb.input_ptr->value: 0;
		reg->value = reg->comb.get_value(reg->value, value2);
	}

	return error;
}

Error reg_set(struct Reg* reg)
{
	Error error;

	if (!reg)
		error = ERROR_NULL_POINTER;
	else if (!reg->input)
		error = WARNING_NO_INPUT;
	else
	{
		reg->to_set = reg->input->value;
	}

	return error;
}

Error comb_reset(struct Reg* reg)
{
	Error error;

	if (!reg)
		error = ERROR_NULL_POINTER;
	else 
	{
		reg->value = EMPTY;
		reg->input = NULL;
	}

	return error;
}

Error reg_reset(struct Reg* reg)
{
	Error error;

	if (!reg)
		error = ERROR_NULL_POINTER;
	else
	{
		if (reg->to_set != EMPTY)
		{
			reg->value = reg->to_set;
			reg->to_set = EMPTY;
		}
		reg->input = NULL;
	}

	return error;
}