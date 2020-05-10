#include <stdlib.h>
#include <string.h>

#include "signal.h"
#include "unit.h"
#include "settings.h"
#include "error.h"
#include "graphics.h"

struct Signal
{
	SignalType type;
	const char* name;
	void* init_struct;
	struct Drawable* drawable;
	bool is_set;
};

struct Signal* signal_new(struct SignalInit* signal_init)
{
	CHECK_IF_NULL(signal_init);
	
	size_t init_struct_size;
	switch (signal_init->type)
	{
	case SIGNAL_FROM_TO:
		init_struct_size = sizeof(struct SignalInitFromTo);
		break;
	case SIGNAL_ALU:
		init_struct_size = sizeof(struct SignalInitALU);
		break;
	case SIGNAL_OTHER:
		init_struct_size = sizeof(struct SignalInitOther);
		break;
	default:
		CRASH_LOG(LOG_UNKNOWN_VALUE);
		//critical_error_set("invalid signal type");
		init_struct_size = 0;
	}
	void* new_init_struct = malloc_s(init_struct_size);
	if (memcpy_s(new_init_struct, init_struct_size, &signal_init->value, init_struct_size))
		CRASH_LOG(LIBRARY_FUNC_FAILURE);
		//critical_error_set("...");
	char* new_name = _strdup(signal_init->name);
	if (!new_name)
		CRASH_LOG(LIBRARY_FUNC_FAILURE);
		//critical_error_set("strdup failed\n");
	struct Drawable* new_drawable = drawable_new_signal(signal_init->drawable_init, new_name);

	struct Signal* new_signal = malloc_s(sizeof(struct Signal));
	*new_signal = (struct Signal){ signal_init->type, new_name, new_init_struct, new_drawable, false };
	return new_signal;
}

var signal_set(struct Signal* signal)
{
	CHECK_IF_NULL(signal);
	var return_value = 0;
	switch (signal->type)
	{
	case SIGNAL_FROM_TO:
	{
		struct SignalInitFromTo* init_struct = signal->init_struct;
		CHECK_IF_NULL(init_struct);
		var value = unit_read(init_struct->from);
		if (value == EMPTY)
			return_value = EMPTY;
		else
		{
			value = init_struct->function(value, *init_struct->mask_ptr);
			/*if (unit_set(init_struct->to, value))
				signal->is_set = true;
			else*/
			if (!unit_set(init_struct->to, value))
				return_value = OUTPUT_ALREADY_SET;
		}
	}
	break;
	case SIGNAL_ALU:
	{
		struct SignalInitALU* init_struct = signal->init_struct;
		CHECK_IF_NULL(init_struct);
		var value_a = unit_read(init_struct->from);
		var value_b = unit_read(init_struct->value_from);
		if (value_a == EMPTY || value_b == EMPTY)
			return_value = EMPTY;
		else
		{
			value_a = init_struct->function(value_a, value_b, *init_struct->mask_ptr);
			//if (unit_set(init_struct->to, value_a))
				//signal->is_set = true;
			//else
			if (!unit_set(init_struct->to, value_a))
				return_value = OUTPUT_ALREADY_SET;
		}
	}
	break;
	case SIGNAL_OTHER:
	{
		struct SignalInitOther* init_struct = signal->init_struct;
		return_value = init_struct->function(init_struct->value_ptr);
	}
	break;
	default:
		CRASH_LOG(LIBRARY_FUNC_FAILURE);
		//critical_error_set("");
	}

	if (!return_value && !signal->is_set)
	{
		signal->is_set = true;
		drawable_set_value(signal->drawable, &signal->is_set);
	}
		
	return return_value;
}

void signal_reset(struct Signal* signal)
{
	CHECK_IF_NULL(signal);
	if (signal->is_set)
	{
		signal->is_set = false;
		drawable_set_value(signal->drawable, &signal->is_set);
	}
}

void signal_draw(struct Signal* signal)
{
	CHECK_IF_NULL(signal);
	if (signal->drawable)
		drawable_set_value(signal->drawable, &signal->is_set);
}

//void signal_show(struct signal* signal)
//{
//	CHECK_IF_NULL(signal);
//	if (signal->drawable)
//		drawable_set_visibility(signal->drawable, true);
//}
//
//void signal_hide(struct signal* signal)
//{
//	CHECK_IF_NULL(signal);
//	if (signal->drawable)
//		drawable_set_visibility(signal->drawable, false);
//}

void signal_set_visibility(struct Signal* signal, bool visibility)
{
	CHECK_IF_NULL(signal);
	if (signal->drawable)
		drawable_set_visibility(signal->drawable, visibility);
}

const char* signal_get_name(struct Signal* signal) 
{
	CHECK_IF_NULL(signal);
	return signal->name;
}


void signal_delete(struct Signal* signal)
{
	if (signal)
	{
		if (signal->type == SIGNAL_OTHER)
		{
			struct SignalInitOther* temp = signal->init_struct;
			free(temp->value_ptr);
		}
		free((char*)signal->name);
		free(signal->init_struct);
		free(signal);
	}
}
