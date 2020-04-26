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
	const Point p_zero = { 0, 0 };

	// ALU
	{
		Point alu_offset = { offset.x + 20, offset.y + 5 };
		struct UnitInit new_unit;

		// REG_AK
		new_unit = (struct UnitInit){
			.position = p_add(alu_offset, POINT(6, 2)),
			.size = REG_SIZE,
			.canvas = canvas,
			.word_length = &cpu->setup.preference.all.word_length
		};
		cpu->components.alu.reg_AK = unit_new_reg(&new_unit, "AK");
		// COMB_WEJA
		new_unit = (struct UnitInit){
			.position = p_add(alu_offset, POINT(6, 4)),
			.size = POINT(REG_WIDTH, 8),
			.canvas = canvas,
			.word_length = &cpu->setup.preference.all.word_length
		};
		cpu->components.alu.comb_weja = unit_new_comb(&new_unit);
		// COMB_WEAK
		new_unit = (struct UnitInit){
			.position = p_zero,
			.size = p_zero,
			.canvas = NULL,
			.word_length = &cpu->setup.preference.all.word_length
		};
		cpu->components.alu.comb_weja = unit_new_comb(&new_unit);

		
		struct DrawableSignalInit drawable_signal_init;
		struct SignalInit signal_init;

		// SIG_WEJA
		drawable_signal_init = (struct DrawableSignalInit){
			.canvas = canvas,
			.arrow.head = p_add(alu_offset, POINT(13, 10)),
			.arrow.tail = p_add(alu_offset, POINT(13, 12)),
			.tag.type = TO_COMB_TYPE,
			.tag.head = p_add(alu_offset, POINT(8, 11)),
			.tag.tail = p_add(alu_offset, POINT(13, 11)),
		};
		signal_init = (struct SignalInit){
			.type = SIGNAL_FROM_TO,
			.name = "weja",
			.fun = NULL,
			.drawable_init = &drawable_signal_init,
			.value.from_to.from = &cpu->components.addr.bus_s,
			.value.from_to.to = &cpu->components.alu.comb_weja
		};
		cpu->components.alu.sig_weja = signal_new(&signal_init);

		// LEFT SIDE SIGNALS
		Point sig_offset = p_add(alu_offset, POINT(0, 2));
		Point sig_length = POINT(5, 0);
		// SIG_IAK
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
			.name = "iak",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.from_to.from = &cpu->components.alu.reg_AK,
			.value.from_to.to = &cpu->components.alu.reg_AK
		};
		cpu->components.alu.sig_iak = signal_new(&signal_init);
		// SIG_WEAK
		sig_offset = p_add(sig_offset, POINT(0, 1));
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
			.name = "weak",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.from_to.from = &cpu->components.alu.comb_weak,
			.value.from_to.to = &cpu->components.alu.reg_AK
		};
		cpu->components.alu.sig_weak = signal_new(&signal_init);
		// SIG_DOD
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
			.type = SIGNAL_ALU,
			.name = "dod",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.alu.from = &cpu->components.alu.comb_weja,
			.value.alu.to = &cpu->components.alu.comb_weak,
			.value.alu.value_from = &cpu->components.alu.reg_AK
		};
		cpu->components.alu.sig_dod = signal_new(&signal_init);
		// SIG_ODE
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
			.type = SIGNAL_ALU,
			.name = "ode",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.alu.from = &cpu->components.alu.comb_weja,
			.value.alu.to = &cpu->components.alu.comb_weak,
			.value.alu.value_from = &cpu->components.alu.reg_AK
		};
		cpu->components.alu.sig_ode = signal_new(&signal_init);
		// SIG_PRZEP
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
			.type = SIGNAL_ALU,
			.name = "przep",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.from_to.from = &cpu->components.alu.comb_weja,
			.value.from_to.to = &cpu->components.alu.comb_weak,
		};
		cpu->components.alu.sig_przep = signal_new(&signal_init);
		// SIG_MNO
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
			.type = SIGNAL_ALU,
			.name = "mno",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.alu.from = &cpu->components.alu.comb_weja,
			.value.alu.to = &cpu->components.alu.comb_weak,
			.value.alu.value_from = &cpu->components.alu.reg_AK
		};
		cpu->components.alu.sig_mno = signal_new(&signal_init);
		// SIG_DZIEL
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
			.type = SIGNAL_ALU,
			.name = "dziel",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.alu.from = &cpu->components.alu.comb_weja,
			.value.alu.to = &cpu->components.alu.comb_weak,
			.value.alu.value_from = &cpu->components.alu.reg_AK
		};
		cpu->components.alu.sig_dziel = signal_new(&signal_init);
		// SIG_SHR
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
			.type = SIGNAL_ALU,
			.name = "shr",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.from_to.from = &cpu->components.alu.comb_weja,
			.value.from_to.to = &cpu->components.alu.comb_weak,
		};
		cpu->components.alu.sig_shr= signal_new(&signal_init);

		// RIGHT SIDE SIGNALS
		sig_offset = p_add(alu_offset, POINT(21, 2));
		sig_length = POINT(5, 0);
		// SIG_IAK
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
			.name = "iak",
			.fun = NULL,	//TODO
			.drawable_init = &drawable_signal_init,
			.value.from_to.from = &cpu->components.alu.reg_AK,
			.value.from_to.to = &cpu->components.alu.reg_AK
		};
		cpu->components.alu.sig_iak = signal_new(&signal_init);
	}
}

struct CPU* cpu_init()
{
	struct CPU* cpu = malloc_s(sizeof(struct CPU));
	//cpu->setup = setup;

	//struct CPUPreference setup[] = {
	struct CPUSetup setup = { 
		.preference.list = {
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
			cpu_preference_init("znaczniki")},
		.basic = {
			.unit_vect = vector_init(sizeof(struct Unit*)),
			.signal_vect = vector_init(sizeof(struct Signal*))}
	};

	struct FileHandler* files_handler = file_handler_init();

	struct Map* setup_map = map_init(sizeof(&setup.preference.list->value));
	size_t setup_size = sizeof(setup.preference.list) / sizeof(*setup.preference.list);
	for (unsigned i = 0; i < setup_size; i++)
	{
		const char* key = setup.preference.list[i].name;
		void* value_ptr = &setup.preference.list[i].value;
		map_push(setup_map, key, &value_ptr);
	}

	if (file_import_setup("MaszynaW.lst", files_handler, setup_map))
	{
		//lista
	}

	map_delete(setup_map);
	file_handler_delete(files_handler);

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