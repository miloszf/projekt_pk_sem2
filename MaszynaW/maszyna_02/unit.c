#include <stdlib.h>
#include <stdbool.h>

#include "unit.h"
#include "settings.h"
#include "error.h"
#include "graphics.h"

typedef enum { REG_TYPE, COMB_TYPE, BUS_TYPE } UnitType;

struct Unit
{
	UnitType type;
	var value;
	var input_value;
	struct Drawable* drawable;
};

struct Unit* unit_init()
{
	return malloc_s(sizeof(struct Unit));
}

struct Unit* unit_new_reg(struct UnitInit* init, const char* name)
{
	CHECK_IF_NULL(init);
	CHECK_IF_NULL(name);
	struct Unit* new_reg = unit_init();
	struct Drawable* new_drawable = drawable_new_reg(init->canvas, init->position, init->size, name);
	*new_reg = (struct Unit){ REG_TYPE, 0, EMPTY, new_drawable };
	return new_reg;
}

struct Unit* unit_new_comb(struct UnitInit* init)
{
	CHECK_IF_NULL(init);
	struct Unit* new_comb = unit_init();
	struct Drawable* new_drawable;
	if (init->size.x && init->size.y)
		new_drawable = drawable_new_comb(init->canvas, init->position, init->size);
	else
		new_drawable = NULL;
	*new_comb = (struct Unit){ COMB_TYPE, EMPTY, EMPTY, new_drawable };
	return new_comb;
}

struct Unit* unit_new_bus(struct UnitInit* init)
{
	CHECK_IF_NULL(init);
	struct Unit* new_bus = unit_init();
	struct Drawable* new_drawable = drawable_new_bus(init->canvas, init->position, init->size);
	*new_bus = (struct Unit){ BUS_TYPE, EMPTY, EMPTY, new_drawable };
	return new_bus;
}

var unit_set(struct Unit* unit, var value)
{
	CHECK_IF_NULL(unit);
	if (unit->input_value == EMPTY)
	{
		if (unit->type != REG_TYPE)
			unit->value = value;
		unit->input_value = value;
		return EMPTY;
	}
	else
		return 0;
}

var unit_immediate_set(struct Unit* unit, var value)
{
	var return_value = (unit->input_value == EMPTY) ? EMPTY : 0;
	unit->value = value;
	unit->input_value = value;
	if (unit->drawable)
		drawable_set_value(unit->drawable, &unit->input_value);
	return return_value;
}

var unit_read(struct Unit* unit)
{
	CHECK_IF_NULL(unit);
	return unit->value;
}

void unit_latch(struct Unit* unit)
{
	CHECK_IF_NULL(unit);
	if (unit->input_value != EMPTY)
	{
		if (unit->type == REG_TYPE)
			unit->value = unit->input_value;
		if (unit->drawable)
			drawable_set_value(unit->drawable, &unit->input_value);
	}
}

void unit_reset(struct Unit* unit)
{
	CHECK_IF_NULL(unit);
	if (unit->type != REG_TYPE && unit->value != EMPTY)
	{
		unit->value = EMPTY;
		if (unit->drawable)
			drawable_set_value(unit->drawable, &unit->value);
	}
	unit->input_value = EMPTY;
}

void unit_restart(struct Unit* unit)
{
	CHECK_IF_NULL(unit);
	unit->value = (unit->type == REG_TYPE) ? 0 : EMPTY;
	unit->input_value = EMPTY;
	if (unit->drawable)
		drawable_set_value(unit->drawable, &unit->value);
}

void unit_show(struct Unit* unit)
{
	CHECK_IF_NULL(unit);
	if (unit->drawable)
		drawable_set_visibility(unit->drawable, true);
}

void unit_hide(struct Unit* unit)
{
	CHECK_IF_NULL(unit);
	if (unit->drawable)
		drawable_set_visibility(unit->drawable, false);
}

void unit_set_visibility(struct Unit* unit, bool visibility)
{
	CHECK_IF_NULL(unit);
	if (unit->drawable)
		drawable_set_visibility(unit->drawable, visibility);
}

void unit_delete(struct Unit* unit)
{
	if (unit)
		free(unit);
}