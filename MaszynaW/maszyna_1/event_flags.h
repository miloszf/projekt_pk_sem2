#ifndef EVENT_FLAGS
#define EVENT_FLAGS

#include <stdbool.h>
#include "settings.h"

struct EventFlags
{
	bool window_resize;
	struct Point window_size;
	bool esc_pressed;
	bool esc_released;
	bool f1_pressed;
	bool f1_released;
};

#endif