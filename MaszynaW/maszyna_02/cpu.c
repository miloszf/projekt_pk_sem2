#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "vector.h"
#include "error.h"
#include "settings.h"


struct CPUPreference
{
	const char* name;
	unsigned char value;
	struct Vector* unit_vect;
	struct Vector* signal_vect;
};

struct CPUPreference cpu_preference_init(const char* name)
{
	check_for_NULL(name);
	size_t name_len = strlen(name) + 1;
	char* new_name = malloc_s(name_len);
	if (strcpy_s(new_name, name_len, name))
		critical_error_set("strcpy_s failed\n");
	struct Vector* unit_vect = vector_init(sizeof(struct Unit*));
	struct Vector* signal_vect = vector_init(sizeof(struct Signal*));
	return (struct CPUPreference){ new_name, 0, unit_vect, signal_vect };
}

void cpu_preference_delete(struct CPUPreference* setup)
{
	check_for_NULL(setup);
	free((char*)setup->name);
	vector_delete(setup->unit_vect);
	vector_delete(setup->signal_vect);
}

#define CPU_SETUP_SIZE 12

struct CPUSetup
{
	struct
	{
		struct Vector* unit_vect;
		struct Vector* signal_vect;
	} basic;

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
		} all;
		struct CPUPreference list[CPU_SETUP_SIZE];
	} preference;
};

void cpu_setup_delete(struct CPUSetup* setup)
{
	if (setup)
		for (int i = 0; i < CPU_SETUP_SIZE; i++)
			cpu_preference_delete(setup->preference.list + i);
}

struct CPUComponents
{
	struct
	{
		struct Unit* reg_AK;
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
		//struct Signal* sig_sa;
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
};

struct CPU
{
	struct CPUSetup setup;
	struct CPUComponents components;

};


void cpu_init_basic(struct CPUComponents* components, struct Vector* unit_vect, struct Vector* signal_vect)
{
	check_for_NULL(components);

	// basic ALU
	//{
	//	Point alu_offset = {}
	//}
}

struct CPU* cpu_init()
{

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
			cpu_preference_init("wyjscie"),
			cpu_preference_init("znaczniki")},
		.basic = {
			.unit_vect = vector_init(sizeof(struct Unit*)),
			.signal_vect = vector_init(sizeof(struct Signal*))}
	};

	struct CPUComponents components;




	struct CPU* cpu = malloc_s(sizeof(struct CPU));
	cpu->setup = setup;
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