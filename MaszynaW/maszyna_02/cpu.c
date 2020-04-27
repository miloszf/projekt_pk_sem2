#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cpu.h"
#include "vector.h"
#include "error.h"
#include "settings.h"
#include "cpu_auxiliary_structs.h"
#include "unit.h"
#include "signal.h"
#include "graphics.h"
#include "map.h"
#include "file.h"

#define REG_WIDTH 15
#define REG_HEIGTH 3
#define REG_SIZE (Point){REG_WIDTH, REG_HEIGTH}

#define POINT(x,y) (Point){x, y}

struct CPU
{
	struct CPUSetup setup;
	struct CPUComponents components;
	struct CPUMemory memory;
	struct
	{
		struct Vector* units;
		struct Vector* signals;
		struct Vector* tags;
	} vector;
};

inline Point p_add(Point a, Point b)
{
	return POINT((a.x + b.x), (a.y + b.y));
}


void cpu_init_alu(struct CPU* cpu, const Point offset, struct Canvas* canvas)
{
	check_for_NULL(cpu);
	check_for_NULL(canvas);
	const Point p_zero = { 0, 0 };

	struct UnitInit new_unit;

	// REG_AK
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(6, 1)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.alu.reg_ak = unit_new_reg(&new_unit, "AK");
	// COMB_WEJA
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(6, 3)),
		.size = POINT(REG_WIDTH, 8),
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.alu.comb_weja = unit_new_comb(&new_unit);
	// COMB_WEAK
	new_unit = (struct UnitInit){
		.position = p_zero,
		.size = p_zero,
		.canvas = NULL,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.alu.comb_weak = unit_new_comb(&new_unit);

	
	struct DrawableSignalInit drawable_signal_init;
	struct SignalInit signal_init;

	// SIG_WEJA
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(13, 10)),
		.arrow.tail = p_add(offset, POINT(13, 12)),
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(offset, POINT(12, 11)),
		.tag.tail = p_add(offset, POINT(8, 11)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "weja",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_s,
		.value.from_to.to = cpu->components.alu.comb_weja
	};
	cpu->components.alu.sig_weja = signal_new(&signal_init);
	
	// LEFT SIDE SIGNALS
	Point sig_offset = p_add(offset, POINT(0, 2));
	Point sig_length = POINT(5, 0);
	// SIG_IAK
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "iak",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.alu.reg_ak,
		.value.from_to.to = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_iak = signal_new(&signal_init);
	// SIG_WEAK
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "weak",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.alu.comb_weak,
		.value.from_to.to = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_weak = signal_new(&signal_init);
	// SIG_DOD
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_ALU,
		.name = "dod",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.alu.from = cpu->components.alu.comb_weja,
		.value.alu.to = cpu->components.alu.comb_weak,
		.value.alu.value_from = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_dod = signal_new(&signal_init);
	// SIG_ODE
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_ALU,
		.name = "ode",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.alu.from = cpu->components.alu.comb_weja,
		.value.alu.to = cpu->components.alu.comb_weak,
		.value.alu.value_from = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_ode = signal_new(&signal_init);
	// SIG_PRZEP
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_ALU,
		.name = "przep",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.alu.comb_weja,
		.value.from_to.to = cpu->components.alu.comb_weak,
	};
	cpu->components.alu.sig_przep = signal_new(&signal_init);
	// SIG_MNO
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_ALU,
		.name = "mno",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.alu.from = cpu->components.alu.comb_weja,
		.value.alu.to = cpu->components.alu.comb_weak,
		.value.alu.value_from = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_mno = signal_new(&signal_init);
	// SIG_DZIEL
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_ALU,
		.name = "dziel",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.alu.from = cpu->components.alu.comb_weja,
		.value.alu.to = cpu->components.alu.comb_weak,
		.value.alu.value_from = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_dziel = signal_new(&signal_init);
	// SIG_SHR
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_ALU,
		.name = "shr",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.alu.comb_weja,
		.value.from_to.to = cpu->components.alu.comb_weak,
	};
	cpu->components.alu.sig_shr= signal_new(&signal_init);

	// RIGHT SIDE SIGNALS
	sig_offset = p_add(offset, POINT(21, 2));
	sig_length = POINT(4, 0);
	// SIG_DAK
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_REG_TYPE,
		.tag.head = sig_offset,
		.tag.tail = p_add(sig_offset, sig_length),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "dak",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.alu.reg_ak,
		.value.from_to.to = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_dak = signal_new(&signal_init);
	// SIG_NEG
	sig_offset = p_add(sig_offset, POINT(0, 2));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = sig_offset,
		.tag.tail = p_add(sig_offset, sig_length),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "neg",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.alu.comb_weja,
		.value.from_to.to = cpu->components.alu.comb_weak,
	};
	cpu->components.alu.sig_neg = signal_new(&signal_init);
	// SIG_LUB
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = sig_offset,
		.tag.tail = p_add(sig_offset, sig_length),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "lub",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.alu.from = cpu->components.alu.comb_weja,
		.value.alu.to = cpu->components.alu.comb_weak,
		.value.alu.value_from = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_lub = signal_new(&signal_init);
	// SIG_I
	sig_offset = p_add(sig_offset, POINT(0, 1));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = sig_offset,
		.tag.tail = p_add(sig_offset, sig_length),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "i",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.alu.from = cpu->components.alu.comb_weja,
		.value.alu.to = cpu->components.alu.comb_weak,
		.value.alu.value_from = cpu->components.alu.reg_ak
	};
	cpu->components.alu.sig_i = signal_new(&signal_init);
	// SIG_WYAK
	sig_offset = p_add(sig_offset, POINT(0, 2));
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(sig_offset, sig_length),
		.tag.tail = sig_offset
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyak",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.alu.comb_weja,
		.value.from_to.to = cpu->components.alu.comb_weak
	};
	cpu->components.alu.sig_wyak = signal_new(&signal_init);
	
	// |---- BASIC ----|
	// UNITS
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.alu.reg_ak);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.alu.comb_weak);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.alu.comb_weja);
	// SIGNALS
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.alu.sig_weja);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.alu.sig_weak);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.alu.sig_dod);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.alu.sig_ode);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.alu.sig_przep);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.alu.sig_wyak);
	// |---- ALU_INC ----|
	vector_push(cpu->setup.all.alu_inc.signal_vect, &cpu->components.alu.sig_iak);
	vector_push(cpu->setup.all.alu_inc.signal_vect, &cpu->components.alu.sig_dak);
	// |---- ALU_LOGIC ----|
	vector_push(cpu->setup.all.alu_logic.signal_vect, &cpu->components.alu.sig_neg);
	vector_push(cpu->setup.all.alu_logic.signal_vect, &cpu->components.alu.sig_lub);
	vector_push(cpu->setup.all.alu_logic.signal_vect, &cpu->components.alu.sig_i);
	// |---- ALU_EXT ----|							  
	vector_push(cpu->setup.all.alu_logic.signal_vect, &cpu->components.alu.sig_mno);
	vector_push(cpu->setup.all.alu_logic.signal_vect, &cpu->components.alu.sig_dziel);
	vector_push(cpu->setup.all.alu_logic.signal_vect, &cpu->components.alu.sig_shr);
}

void cpu_init_mem(struct CPU* cpu, const Point offset, struct Canvas* canvas)
{
	check_for_NULL(cpu);
	check_for_NULL(canvas);
	const Point p_zero = { 0, 0 };

	struct UnitInit new_unit;

	// REG_A
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(0, 2)),
		.size = POINT(18, REG_HEIGTH),
		.canvas = canvas,
		.word_length = &cpu->setup.all.addr_length.value
	};
	cpu->components.mem.reg_a = unit_new_reg(&new_unit, "A");
	// REG_S
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(0, 13)),
		.size = POINT(18, REG_HEIGTH),
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.mem.reg_s = unit_new_reg(&new_unit, "S");

	struct DrawableSignalInit drawable_signal_init;
	struct SignalInit signal_init;
	// SIG_WEA
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(9, 2)),
		.arrow.tail = p_add(offset, POINT(9, 0)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(10, 1)),
		.tag.tail = p_add(offset, POINT(14, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wea",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_a,
		.value.from_to.to = cpu->components.mem.reg_a
	};
	cpu->components.mem.sig_wea = signal_new(&signal_init);
	// SIG_WYS
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(8, 17)),
		.arrow.tail = p_add(offset, POINT(8, 15)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(7, 16)),
		.tag.tail = p_add(offset, POINT(3, 16)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wys",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.mem.reg_s,
		.value.from_to.to = cpu->components.addr.bus_s
	};
	cpu->components.mem.sig_wys = signal_new(&signal_init);
	// SIG_WES
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(10, 15)),
		.arrow.tail = p_add(offset, POINT(10, 17)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(11, 16)),
		.tag.tail = p_add(offset, POINT(15, 16)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wes",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_s,
		.value.from_to.to = cpu->components.mem.reg_s
	};
	cpu->components.mem.sig_wes = signal_new(&signal_init);
	// SIG_CZYT
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(offset, POINT(18, 7)),
		.tag.tail = p_add(offset, POINT(22, 7)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "czyt",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		//.value.from_to.from = cpu->components.addr.bus_s,
		//.value.from_to.to = cpu->components.mem.reg_s
	};
	cpu->components.mem.sig_czyt = signal_new(&signal_init);
	// SIG_PISZ
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_COMB_TYPE,
		.tag.head = p_add(offset, POINT(18, 10)),
		.tag.tail = p_add(offset, POINT(22, 10)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "pisz",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		//.value.from_to.from = cpu->components.addr.bus_s,
		//.value.from_to.to = cpu->components.mem.reg_s
	};
	cpu->components.mem.sig_pisz = signal_new(&signal_init);

	// |---- BASIC ----|
	// UNITS
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.mem.reg_a);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.mem.reg_s);
	// SIGNALS
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.mem.sig_czyt);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.mem.sig_pisz);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.mem.sig_wea);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.mem.sig_wys);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.mem.sig_wes);
}

void cpu_init_addr(struct CPU* cpu, const Point offset, struct Canvas* canvas)
{
	check_for_NULL(cpu);
	check_for_NULL(canvas);
	const Point p_zero = { 0, 0 };

	struct UnitInit new_unit;
	// BUS_A
	new_unit = (struct UnitInit){
		.position = offset,
		.size = POINT(68, 0),
		.canvas = canvas,
		.word_length = &cpu->setup.all.addr_length.value
	};
	cpu->components.addr.bus_a = unit_new_bus(&new_unit);
	// BUS_S
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(0, 17)),
		.size = POINT(68, 0),
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.addr.bus_s = unit_new_bus(&new_unit);
	// BUS_AS
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(18, 0)),
		.size = POINT(0, 18),
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.addr.bus_as = unit_new_bus(&new_unit);
	// REG_L
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.addr_length.value
	};
	cpu->components.addr.reg_l = unit_new_reg(&new_unit, "L");
	// REG_I
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(3, 13)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.addr.reg_i = unit_new_reg(&new_unit, "I");

	struct DrawableSignalInit drawable_signal_init;
	struct SignalInit signal_init;

	// SIG_WYL
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(7, 0)),
		.arrow.tail = p_add(offset, POINT(7, 2)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(6, 1)),
		.tag.tail = p_add(offset, POINT(2, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyl",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.reg_l,
		.value.from_to.to = cpu->components.addr.bus_a
	};
	cpu->components.addr.sig_wyl = signal_new(&signal_init);
	// SIG_WEL
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(9, 2)),
		.arrow.tail = p_add(offset, POINT(9, 0)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(10, 1)),
		.tag.tail = p_add(offset, POINT(14, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wel",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_a,
		.value.from_to.to = cpu->components.addr.reg_l
	};
	cpu->components.addr.sig_wel = signal_new(&signal_init);
	// SIG_WEI
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(10, 15)),
		.arrow.tail = p_add(offset, POINT(10, 17)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(9, 16)),
		.tag.tail = p_add(offset, POINT(5, 16)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wei",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		//.value.from_to.from = cpu->components.addr.reg_l,
		//.value.from_to.to = cpu->components.addr.bus_a
	};
	cpu->components.addr.sig_wei = signal_new(&signal_init);
	// SIG_WYLS
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(2, 17)),
		.arrow.tail = p_add(offset, POINT(2, 4)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(3, 6)),
		.tag.tail = p_add(offset, POINT(7, 6)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyls",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.reg_l,
		.value.from_to.to = cpu->components.addr.bus_s
	};
	cpu->components.addr.sig_wyls = signal_new(&signal_init);
	// SIG_WYAD
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(16, 0)),
		.arrow.tail = p_add(offset, POINT(16, 13)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(15, 10)),
		.tag.tail = p_add(offset, POINT(11, 10)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyad",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.reg_i,
		.value.from_to.to = cpu->components.addr.bus_a
	};
	cpu->components.addr.sig_wyad = signal_new(&signal_init);
	// SIG_IL
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(14, 4)),
		.arrow.tail = p_add(offset, POINT(14, 8)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(13, 8)),
		.tag.tail = p_add(offset, POINT(10, 8)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "il",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.reg_i,
		.value.from_to.to = cpu->components.addr.reg_i
	};
	cpu->components.addr.sig_il = signal_new(&signal_init);
	// SIG_AS
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(19, 1)),
		.tag.tail = p_add(offset, POINT(22, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "as",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		//.value.from_to.from = cpu->components.addr.reg_i,
		//.value.from_to.to = cpu->components.addr.reg_i
	};
	cpu->components.addr.sig_as = signal_new(&signal_init);

	// |---- BASIC ----|
	// UNITS
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.addr.bus_a);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.addr.bus_s);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.addr.bus_as);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.addr.reg_i);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.addr.reg_l);
	// SIGNALS
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.addr.sig_wyl);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.addr.sig_wel);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.addr.sig_wei);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.addr.sig_wyls);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.addr.sig_wyad);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.addr.sig_il);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.addr.sig_as);
}

void cpu_init_xy(struct CPU* cpu, Point offset, struct Canvas* canvas)
{
	check_for_NULL(cpu);
	check_for_NULL(canvas);
	const Point p_zero = { 0, 0 };

	struct UnitInit new_unit;

	// REG_X
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.xy.reg_x = unit_new_reg(&new_unit, "X");

	struct DrawableSignalInit drawable_signal_init;
	struct SignalInit signal_init;

	// SIG_WYX
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(7, 0)),
		.arrow.tail = p_add(offset, POINT(7, 2)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(6, 1)),
		.tag.tail = p_add(offset, POINT(2, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyx",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.xy.reg_x,
		.value.from_to.to = cpu->components.addr.bus_s
	};
	cpu->components.xy.sig_wyx = signal_new(&signal_init);
	// SIG_WEX
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(9, 2)),
		.arrow.tail = p_add(offset, POINT(9, 0)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(10, 1)),
		.tag.tail = p_add(offset, POINT(14, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wex",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_s,
		.value.from_to.to = cpu->components.xy.reg_x,
	};
	cpu->components.xy.sig_wex = signal_new(&signal_init);

	offset = p_add(offset, POINT(16, 0));
	// REG_Y
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.xy.reg_y = unit_new_reg(&new_unit, "Y");

	// SIG_WYY
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(7, 0)),
		.arrow.tail = p_add(offset, POINT(7, 2)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(6, 1)),
		.tag.tail = p_add(offset, POINT(2, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyy",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.xy.reg_y,
		.value.from_to.to = cpu->components.addr.bus_s
	};
	cpu->components.xy.sig_wyy = signal_new(&signal_init);
	// SIG_WEY
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(9, 2)),
		.arrow.tail = p_add(offset, POINT(9, 0)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(10, 1)),
		.tag.tail = p_add(offset, POINT(14, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wey",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_s,
		.value.from_to.to = cpu->components.xy.reg_y,
	};
	cpu->components.xy.sig_wey = signal_new(&signal_init);

	// |---- XY ----|
	// UNITS
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.xy.reg_x);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.xy.reg_y);
	// SIGNALS
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.xy.sig_wex);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.xy.sig_wey);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.xy.sig_wyx);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.xy.sig_wyy);
}

void cpu_init_stack(struct CPU* cpu, const Point offset, struct Canvas* canvas)
{
	check_for_NULL(cpu);
	check_for_NULL(canvas);
	const Point p_zero = { 0, 0 };

	struct UnitInit new_unit;
	// REG_WS
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(4, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		// TODO nie bêdzie dzia³aæ
		.word_length = &cpu->setup.all.addr_length.value
	};
	cpu->components.stack.reg_ws = unit_new_reg(&new_unit, "WS");

	struct DrawableSignalInit drawable_signal_init;
	struct SignalInit signal_init;

	// SIG_WYWS
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(10, 0)),
		.arrow.tail = p_add(offset, POINT(10, 2)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(9, 1)),
		.tag.tail = p_add(offset, POINT(5, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyws",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.stack.reg_ws,
		.value.from_to.to = cpu->components.addr.bus_a
	};
	cpu->components.stack.sig_wyws = signal_new(&signal_init);
	// SIG_WEWS
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(12, 2)),
		.arrow.tail = p_add(offset, POINT(12, 0)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(13, 1)),
		.tag.tail = p_add(offset, POINT(17, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wews",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_a,
		.value.from_to.to = cpu->components.stack.reg_ws
	};
	cpu->components.stack.sig_wews = signal_new(&signal_init);
	// SIG_IWS
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(3, 3)),
		.tag.tail = p_add(offset, POINT(0, 3)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "iws",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.stack.reg_ws,
		.value.from_to.to = cpu->components.stack.reg_ws
	};
	cpu->components.stack.sig_iws = signal_new(&signal_init);
	// SIG_DWS
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(19, 3)),
		.tag.tail = p_add(offset, POINT(22, 3)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "dws",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.stack.reg_ws,
		.value.from_to.to = cpu->components.stack.reg_ws
	};
	cpu->components.stack.sig_dws = signal_new(&signal_init);

	// |---- STACK ----|
	// UNITS
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.stack.reg_ws);
	// SIGNALS
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.stack.sig_dws);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.stack.sig_iws);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.stack.sig_wews);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.stack.sig_wyws);
}

void cpu_init_io(struct CPU* cpu, Point offset, struct Canvas* canvas)
{
	check_for_NULL(cpu);
	check_for_NULL(canvas);
	const Point p_zero = { 0, 0 };

	struct UnitInit new_unit;

	// REG_RB
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.io.reg_rb = unit_new_reg(&new_unit, "RB");

	struct DrawableSignalInit drawable_signal_init;
	struct SignalInit signal_init;

	// SIG_WYRB
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(7, 0)),
		.arrow.tail = p_add(offset, POINT(7, 2)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(6, 1)),
		.tag.tail = p_add(offset, POINT(2, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyrb",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.io.reg_rb,
		.value.from_to.to = cpu->components.addr.bus_s
	};
	cpu->components.io.sig_wyrb = signal_new(&signal_init);
	// SIG_WERB
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(9, 2)),
		.arrow.tail = p_add(offset, POINT(9, 0)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(10, 1)),
		.tag.tail = p_add(offset, POINT(14, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "werb",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_s,
		.value.from_to.to = cpu->components.io.reg_rb,
	};
	cpu->components.io.sig_werb = signal_new(&signal_init);

	offset = p_add(offset, POINT(16, 0));
	// REG_G
	new_unit = (struct UnitInit){
		.position = p_add(offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.word_length.value
	};
	cpu->components.io.reg_g = unit_new_reg(&new_unit, "G");

	// SIG_WYG
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(7, 0)),
		.arrow.tail = p_add(offset, POINT(7, 2)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(6, 1)),
		.tag.tail = p_add(offset, POINT(2, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyg",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.io.reg_g,
		.value.from_to.to = cpu->components.addr.bus_s
	};
	cpu->components.io.sig_wyg = signal_new(&signal_init);
	// SIG_START
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(9, 2)),
		.arrow.tail = p_add(offset, POINT(9, 0)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(10, 1)),
		.tag.tail = p_add(offset, POINT(15, 1)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "start",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_s,
		.value.from_to.to = cpu->components.io.reg_g,
	};
	cpu->components.io.sig_start = signal_new(&signal_init);

	// |---- IO ----|
	// UNITS
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.io.reg_rb);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.io.reg_g);
	// SIGNALS
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.io.sig_start);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.io.sig_werb);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.io.sig_wyg);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.io.sig_wyrb);
}

void set_visibility(struct CPUSetup* setup)
{
	check_for_NULL(setup);
	for (int i = 0; i < CPU_SETUP_SIZE; i++)
	{
		//bool visibility = setup->list[i].value;
		// DEBUG
		bool visibility = true;
		int unit_vect_size = vector_size(setup->list[i].unit_vect);
		for (int index = 0; index < unit_vect_size; index++)
		{
			struct Unit** unit_ptr = vector_read(setup->list[i].unit_vect, index);
			check_for_NULL(unit_ptr);
			unit_set_visibility(*unit_ptr, visibility);
		}
		int signal_vect_size = vector_size(setup->list[i].signal_vect);
		for (int index = 0; index < signal_vect_size; index++)
		{
			struct Signal** signal_ptr = vector_read(setup->list[i].signal_vect, index);
			check_for_NULL(signal_ptr);
			signal_set_visibility(*signal_ptr, visibility);
		}
	}
}

void cpu_init_intr(struct CPU* cpu, const Point offset, struct Canvas* canvas)
{
	check_for_NULL(cpu);
	check_for_NULL(canvas);
	const Point p_zero = { 0, 0 };

	struct UnitInit new_unit;
	Point reg_offset = offset;
	// REG_RZ
	new_unit = (struct UnitInit){
		.position = p_add(reg_offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.addr_length.value
	};
	cpu->components.intr.reg_rz = unit_new_reg(&new_unit, "RZ");
	// REG_RM
	reg_offset = p_add(offset, POINT(0, 2));
	new_unit = (struct UnitInit){
		.position = p_add(reg_offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.addr_length.value
	};
	cpu->components.intr.reg_rm = unit_new_reg(&new_unit, "RM");
	// REG_RP
	reg_offset = p_add(offset, POINT(16, 0));
	new_unit = (struct UnitInit){
		.position = p_add(reg_offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.addr_length.value
	};
	cpu->components.intr.reg_rp = unit_new_reg(&new_unit, "RP");
	// REG_AP
	reg_offset = p_add(reg_offset, POINT(0, 2));
	new_unit = (struct UnitInit){
		.position = p_add(reg_offset, POINT(1, 2)),
		.size = REG_SIZE,
		.canvas = canvas,
		.word_length = &cpu->setup.all.addr_length.value
	};
	cpu->components.intr.reg_ap = unit_new_reg(&new_unit, "AP");

	struct DrawableSignalInit drawable_signal_init;
	struct SignalInit signal_init;

	// SIG_WYRM
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(7, 8)),
		.arrow.tail = p_add(offset, POINT(7, 6)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(6, 7)),
		.tag.tail = p_add(offset, POINT(2, 7)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyrm",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.intr.reg_rm,
		.value.from_to.to = cpu->components.addr.bus_a
	};
	cpu->components.intr.sig_wyrm = signal_new(&signal_init);
	// SIG_WERM
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(9, 8)),
		.arrow.tail = p_add(offset, POINT(9, 6)),
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(10, 7)),
		.tag.tail = p_add(offset, POINT(14, 7)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "werm",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.addr.bus_a,
		.value.from_to.to = cpu->components.intr.reg_rm,
	};
	cpu->components.intr.sig_werm = signal_new(&signal_init);
	// SIG_WYAP
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_add(offset, POINT(24, 8)),
		.arrow.tail = p_add(offset, POINT(24, 6)),
		.tag.type = TO_BUS_TYPE,
		.tag.head = p_add(offset, POINT(23, 7)),
		.tag.tail = p_add(offset, POINT(19, 7)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "wyap",
		.fun = NULL,
		.drawable_init = &drawable_signal_init,
		.value.from_to.from = cpu->components.intr.reg_ap,
		.value.from_to.to = cpu->components.addr.bus_a
	};
	cpu->components.intr.sig_wyap = signal_new(&signal_init);
	// SIG_RINT
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(33, 3)),
		.tag.tail = p_add(offset, POINT(38, 3)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "rint",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		//.value.from_to.from = cpu->components.alu.reg_ak,
		//.value.from_to.to = cpu->components.alu.reg_ak
	};
	cpu->components.intr.sig_rint = signal_new(&signal_init);
	// SIG_ENI
	drawable_signal_init = (struct DrawableSignalInit){
		.canvas = canvas,
		.arrow.head = p_zero,
		.arrow.tail = p_zero,
		.tag.type = TO_REG_TYPE,
		.tag.head = p_add(offset, POINT(33, 5)),
		.tag.tail = p_add(offset, POINT(38, 5)),
	};
	signal_init = (struct SignalInit){
		.type = SIGNAL_FROM_TO,
		.name = "eni",
		.fun = NULL,	//TODO
		.drawable_init = &drawable_signal_init,
		//.value.from_to.from = cpu->components.alu.reg_ak,
		//.value.from_to.to = cpu->components.alu.reg_ak
	};
	cpu->components.intr.sig_eni = signal_new(&signal_init);

	// |---- INTR ----|
	// UNITS
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.intr.reg_ap);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.intr.reg_rm);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.intr.reg_rz);
	vector_push(cpu->setup.all.basic.unit_vect, &cpu->components.intr.reg_rp);
	// SIGNALS
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.intr.sig_eni);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.intr.sig_rint);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.intr.sig_werm);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.intr.sig_wyrm);
	vector_push(cpu->setup.all.basic.signal_vect, &cpu->components.intr.sig_wyap);
}

struct CPU* cpu_init(struct Canvas* canvas)
{
	check_for_NULL(canvas);

	struct CPU* cpu = malloc_s(sizeof(struct CPU));

	//struct CPUPreference setup[] = {
	struct CPUSetup setup = { 
		.list = {
			cpu_preference_init("adres"),
			cpu_preference_init("kod"),
			cpu_preference_init("polaczenie"),
			cpu_preference_init("inkrementacja"),
			cpu_preference_init("logiczne"),
			cpu_preference_init("arytmetyczne"),
			cpu_preference_init("stos"),
			cpu_preference_init("rejestrx"),
			cpu_preference_init("rejestry"),
			cpu_preference_init("przerwania"),
			cpu_preference_init("wejscie"),
			cpu_preference_init("znaczniki"),
			cpu_preference_init(""),
		}
	};
	cpu->setup = setup;

	cpu_init_alu(cpu, POINT(20, 13), canvas);
	cpu_init_mem(cpu, POINT(47, 8), canvas);
	cpu_init_addr(cpu, POINT(1, 8), canvas);
	cpu_init_xy(cpu, POINT(1, 25), canvas);
	cpu_init_stack(cpu, POINT(22, 8), canvas);
	cpu_init_io(cpu, POINT(37, 25), canvas);
	cpu_init_intr(cpu, POINT(1, 0), canvas);


	set_visibility(&setup);

	//struct FileHandler* files_handler = file_handler_init();

	//struct Map* setup_map = map_init(sizeof(&setup.preference.list->value));
	//size_t setup_size = sizeof(setup.preference.list) / sizeof(*setup.preference.list);
	//for (unsigned i = 0; i < setup_size; i++)
	//{
	//	const char* key = setup.preference.list[i].name;
	//	void* value_ptr = &setup.preference.list[i].value;
	//	map_push(setup_map, key, &value_ptr);
	//}

	//if (file_import_setup("MaszynaW.lst", files_handler, setup_map))
	//{
	//	//lista
	//}

	//map_delete(setup_map);
	//file_handler_delete(files_handler);

	//struct CPUComponents components;

	/* inicjalizacja unitów */ 
	// ...

	/* inicjalizacja sygna³ów */
	// ...

	/* za³adowanie ustawieñ z pliku */
	// ...

	/* inicjalizacja cpu zgodnie z ustawieniami 
	   utworzenie wektora obs³ugiwanych */
	// ...


	return cpu;
}


void cpu_delete(struct CPU* cpu)
{
	if (cpu)
	{
		cpu_setup_delete(&cpu->setup);
		free(cpu);
	}
}