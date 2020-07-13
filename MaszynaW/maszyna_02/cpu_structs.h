#ifndef CPU_STRUCTS_H
#define CPU_STRUCTS_H

#include <stdbool.h>
#include "cpu.h"
#include "settings.h"

struct CPUPreference
{
	const char* name;
	unsigned char value;
	struct Vector* unit_vect;
	struct Vector* signal_vect;
};

#define CPU_SETUP_SIZE 13
#define CPU_TAGS_NUMBER 4
#define CPU_INTERRUPTS_NUMBER 4

struct CPUSetup
{
	union
	{
		struct {
			struct CPUPreference addr_length;
			struct CPUPreference code_length;
			struct CPUPreference bus_link;
			struct CPUPreference alu_inc;
			struct CPUPreference alu_logic;
			struct CPUPreference alu_ext;
			struct CPUPreference stack;
			struct CPUPreference reg_x;
			struct CPUPreference reg_y;
			struct CPUPreference interrupts;
			struct CPUPreference io;
			struct CPUPreference tags;
			struct CPUPreference basic;
		} all;
		struct CPUPreference list[CPU_SETUP_SIZE];
	};// preference;
};

typedef enum { TAG_Z, TAG_ZAK, TAG_V, TAG_INT } CPUTagType;
struct CPUTag
{
	const char* name;
	bool value;
	CPUTagType type;
};

struct CPUComponents
{
	struct
	{
		struct Unit* reg_ak;
		struct Unit* comb_weja;
		struct Unit* comb_weak;
		struct Signal* sig_weja;
		struct Signal* sig_shr;
		struct Signal* sig_dziel;
		struct Signal* sig_mno;
		struct Signal* sig_przep;
		struct Signal* sig_ode;
		struct Signal* sig_dod;
		struct Signal* sig_weak;
		struct Signal* sig_iak;
		struct Signal* sig_wyak;
		struct Signal* sig_dak;
		struct Signal* sig_neg;
		struct Signal* sig_lub;
		struct Signal* sig_i;
	} alu;

	struct {
		struct Unit* reg_a;
		struct Unit* reg_s;
		struct Signal* sig_czyt;
		struct Signal* sig_pisz;
		struct Signal* sig_wea;
		struct Signal* sig_wys;
		struct Signal* sig_wes;
	} mem;

	struct {
		struct Unit* reg_i;
		struct Unit* reg_l;
		struct Unit* bus_s;
		struct Unit* bus_a;
		struct Unit* bus_as;
		struct Signal* sig_wyl;
		struct Signal* sig_wel;
		struct Signal* sig_wyls;
		struct Signal* sig_il;
		struct Signal* sig_wyad;
		struct Signal* sig_wei;
		struct Signal* sig_as;
		struct Signal* sig_sa;
		struct Signal* sig_stop;
	} addr;

	struct {
		struct Unit* reg_x;
		struct Unit* reg_y;
		struct Signal* sig_wyx;
		struct Signal* sig_wex;
		struct Signal* sig_wyy;
		struct Signal* sig_wey;
	} xy;

	struct {
		struct Unit* reg_ws;
		struct Signal* sig_wyws;
		struct Signal* sig_wews;
		struct Signal* sig_iws;
		struct Signal* sig_dws;
	} stack;

	struct {
		struct Unit* reg_rb;
		struct Unit* reg_g;
		struct Signal* sig_wyrb;
		struct Signal* sig_werb;
		struct Signal* sig_wyg;
		struct Signal* sig_start;
	} io;

	struct {
		struct Unit* reg_rz;
		struct Unit* reg_rm;
		struct Unit* reg_rp;
		struct Unit* reg_ap;
		struct Signal* sig_wyrm;
		struct Signal* sig_werm;
		struct Signal* sig_rint;
		struct Signal* sig_eni;
		struct Signal* sig_wyap;
	} intr;

	union
	{
		struct
		{
			struct CPUTag tag_int;
			struct CPUTag tag_v;
			struct CPUTag tag_z;
			struct CPUTag tag_zak;
		} all;
		struct CPUTag list[CPU_TAGS_NUMBER];
	}tags;

};

struct CPUMemory
{
	var* memory_array;
	const var* addr_len;
	struct Drawable* drawable;
	struct Vector* instr_names_vect;
};

struct CPUWord
{
	var word_len;
	var addr_len;
	var word_mask;
	var addr_mask;
	var char_mask;
	var intr_mask;
	var instr_num;
};

struct CPURuntime
{
	struct Instruction* current_instr;
	struct Tick* current_tick;
	bool stop;
};


struct CPUPeripherals
{
	struct CPU_IO_Handler cpu_io_handler;
	struct Drawable* buttons_array[CPU_INTERRUPTS_NUMBER];
	var buttons_set;
};

struct CPU
{
	struct CPUSetup setup;
	struct CPUComponents components;
	struct CPUWord word;
	struct CPUMemory* memory;
	struct
	{
		// do usuniêcia?
		struct Vector* units;
		struct Vector* signals;
		struct Vector* instructions;
	} vector;
	struct CPURuntime runtime;
	struct CPUPeripherals peripherals;
	//struct Drawable* frame;
};

struct CPUTag cpu_tag_init(const char* name, CPUTagType type);
bool cpu_tag_update(struct CPUTag* tag, struct CPU* cpu);
void cpu_tag_delete(struct CPUTag* tag);

struct CPUMemory* cpu_memory_init(struct Canvas* canvas, const Point position, const var* addr_length);
void cpu_memory_update(struct CPUMemory* memory, struct Vector* instr_vect);
void cpu_memory_scroll(struct CPUMemory* memory, var offset);
void cpu_memory_delete(struct CPUMemory*);

struct CPUWord cpu_word_init();
void  cpu_word_update(struct CPUWord* word, var code_length, var address_length, struct Vector* instr_vect);

void cpu_peripherals_update_buttons(var interrupts, struct Drawable** buttons_array);

#endif