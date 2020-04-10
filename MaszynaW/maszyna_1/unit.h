#ifndef UNIT
#define UNIT

#include "settings.h"

struct Unit {
	uint value;
	struct Unit** children;
	struct Unit* parent;

	void (*set_value)(struct Unit*);
};

#endif