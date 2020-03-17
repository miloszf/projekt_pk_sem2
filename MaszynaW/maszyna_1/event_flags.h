#ifndef EVENT_FLAGS_H
#define EVENT_FLAGS_H

#include <stdbool.h>
#include "settings.h"

struct Eventflags
{
	bool window_resize;
	struct Point window_size;
	struct Keys
	{
		bool esc;
		bool f1;
	} pressed, released;
};



#endif