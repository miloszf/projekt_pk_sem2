#ifndef CONTROL_UNIT
#define CONTROL_UNIT

#include "unit.h"
#include "error.h"

struct ControlUnit
{
	struct Unit* regs;
	struct Unit* combs;
};

Error control_unit_init(struct ControlUnit* cunit);

#endif