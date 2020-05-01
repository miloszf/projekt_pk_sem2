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
	var (*execute)(void*);
	void* arguments;
	struct Drawable* drawable;
};

struct Signal* signal_new(struct SignalInit* signal_init)
{
	check_for_NULL(signal_init);
	
	size_t arguments_size;
	switch (signal_init->type)
	{
	case SIGNAL_FROM_TO:
		arguments_size = sizeof(struct SignalInitFromTo);
		break;
	case SIGNAL_ALU:
		arguments_size = sizeof(struct SignalInitALU);
		break;
	default:
		critical_error_set("invalid signal type");
		arguments_size = 0;
	}
	void* new_arguments = malloc_s(arguments_size);
	size_t name_len = strlen(signal_init->name) + 1;
	char* new_name = malloc_s(name_len);
	if (strcpy_s(new_name, name_len, signal_init->name))
		critical_error_set("strcpy_s failed");
	struct Drawable* new_drawable = drawable_new_signal(signal_init->drawable_init, signal_init->name);

	struct Signal* new_signal = malloc_s(sizeof(struct Signal));
	*new_signal = (struct Signal){ signal_init->type, new_name, signal_init->fun, new_arguments, new_drawable };
	return new_signal;
}

var signal_set(struct Signal* signal)
{
	check_for_NULL(signal);
	return signal->execute(signal->arguments);
}

void signal_reset(struct Signal* signal)
{

}

//void signal_draw(struct Signal* signal)
//{
//	check_for_NULL(signal);
//	if (signal->drawable)
//		drawable_set_value(signal->value);
//}

void signal_show(struct Signal* signal)
{
	check_for_NULL(signal);
	if (signal->drawable)
		drawable_set_visibility(signal->drawable, true);
}

void signal_hide(struct Signal* signal)
{
	check_for_NULL(signal);
	if (signal->drawable)
		drawable_set_visibility(signal->drawable, false);
}

void signal_set_visibility(struct Signal* signal, bool visibility)
{
	check_for_NULL(signal);
	if (signal->drawable)
		drawable_set_visibility(signal->drawable, visibility);
}

const char* signal_get_name(struct Signal* signal) 
{
	check_for_NULL(signal);
	return signal->name;
}


void signal_delete(struct Signal* signal)
{
	if (signal)
	{
		free((char*)signal->name);
		free(signal->arguments);
		free(signal);
	}
}
