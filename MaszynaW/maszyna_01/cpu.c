#include <stdlib.h>

#include "cpu.h"
#include "error.h"
#include "reg.h"
#include "reg_functions.h"
#include "signal.h"
#include "file_handling.h"


//typedef struct Reg sig;
//

#define DEFAULT_ADDRESS_LENGTH 5
#define DEFAULT_CODE_LENGTH 3
#define MAX_ADDRESS_LENGTH 16
#define MAX_CODE_LENGTH 8

void setup_init(struct CPUSetup* setup)
{
	if (setup)
	{
		//struct CPUSetup s = { DEFAULT_ADDRESS_LENGTH , DEFAULT_CODE_LENGTH };
		*setup = (struct CPUSetup){ DEFAULT_ADDRESS_LENGTH , DEFAULT_CODE_LENGTH };
	}
}

struct CPU
{
	struct CPUSetup setup;
	struct CPUTags tags;
	struct InstructionList* il_head;
	struct Signal* signals;
};

/*
Error init(struct CPU* cpu)
{
	if (!cpu)
		return ERROR_NULL_POINTER;

	typedef struct Reg* preg;

	struct {
		struct {
			preg ak;
			preg weja;
			preg shr;
			preg dziel;
			preg mno;
			preg ode;
			preg dod;
			preg weak;
			preg iak;
			preg dak;
			preg neg;
			preg lub;
			preg i;
		} alu;

		struct {
			preg reg_a;
			preg reg_s;
			preg czyt;
			preg pisz;
		} mem;

		struct {
			preg l;
			preg i;
			preg il;
			preg mag_a;
			preg mag_s;
			//preg as;
		} addr;

		struct {
			preg x;
			preg y;
		} xy;

		struct {
			preg ws;
			preg iws;
			preg dws;
		} stack;

		struct {
			preg rb;
			preg g;
		} io;

		struct {
			preg rz;
			preg rm;
			preg rp;
			preg ap;
			//preg rint; ???
			//preg eni;	 ???
		} intrp;

		preg stop;
	} regs;

	regs.alu.ak	= reg_init(reg_set, reg_reset);
	regs.alu.weja = reg_init(comb_set, comb_reset, &return_first);
	regs.alu.shr = reg_init(comb_set, comb_reset, &return_first);
	regs.alu.dziel = reg_init(comb_set, comb_reset, &divide, &regs.alu.ak);
	regs.alu.mno = reg_init(comb_set, comb_reset, &multiply, &regs.alu.ak);
	regs.alu.dod = reg_init(comb_set, comb_reset, &add, &regs.alu.ak);
	regs.alu.ode = reg_init(comb_set, comb_reset, &subtract, &regs.alu.ak);
	regs.alu.weak = reg_init(comb_set, comb_reset, &return_first);
	regs.alu.iak = reg_init(comb_set, comb_reset, &increment, &regs.alu.ak);
	regs.alu.dak = reg_init(comb_set, comb_reset, &decrement, &regs.alu.ak);
	regs.alu.neg = reg_init(comb_set, comb_reset, &bitwise_complement);
	regs.alu.lub = reg_init(comb_set, comb_reset, &bitwise_or);
	regs.alu.i = reg_init(comb_set, comb_reset, &bitwise_and);

	regs.mem.reg_a = reg_init(reg_set, reg_reset);
	// TODO
	// regs.mem.reg_s =
	// regs.mem.czyt = 
	// regs.mem.pisz = 

	regs.addr.l = reg_init(reg_set, reg_reset);
	regs.addr.i = reg_init(reg_set, reg_reset);
	regs.addr.il = reg_init(comb_set, comb_reset, &increment, &regs.addr.l);
	regs.addr.mag_a = reg_init(comb_set, comb_reset, &return_first);
	regs.addr.mag_s = reg_init(comb_set, comb_reset, &return_first);
	//regs.addr.as = reg_init(comb_set, comb_reset, &return_first);

	regs.xy.x = reg_init(reg_set, reg_reset);
	regs.xy.y = reg_init(reg_set, reg_reset);

	regs.stack.ws = reg_init(reg_set, reg_reset);
	regs.stack.iws = reg_init(comb_set, comb_reset, &increment, &regs.stack.iws);
	regs.stack.dws = reg_init(comb_set, comb_reset, &decrement, &regs.stack.dws);

	// TODO
	// regs.io.rb = 
	// regs.io.g = 

	// TODO
	// regs.intrp.rz = 
	// regs.intrp.rp = 
	// regs.intrp.ap = 
	regs.intrp.rm = reg_init(reg_set, reg_reset);


	// TODO
	// regs.stop = 

	// TODO
	// przerobiæ na funkcjê fun(...) zwracaj¹c¹ listê
	struct Signal* signals[] = {
		// ALU
		signal_init("weja", &regs.addr.mag_s, &regs.alu.weja),
		signal_init("shr", &regs.alu.weja, &regs.alu.weak, &regs.alu.shr),
		signal_init("dziel", &regs.alu.weja, &regs.alu.weak, &regs.alu.dziel),
		signal_init("mno", &regs.alu.weja, &regs.alu.weak, &regs.alu.mno),
		signal_init("ode", &regs.alu.weja, &regs.alu.weak, &regs.alu.ode),
		signal_init("dod", &regs.alu.weja, &regs.alu.weak, &regs.alu.dod),
		signal_init("weak", &regs.alu.weak, &regs.alu.ak),
		signal_init("iak", &regs.alu.iak, &regs.alu.ak),
		signal_init("dak", &regs.alu.dak, &regs.alu.ak),
		signal_init("neg", &regs.alu.weja, &regs.alu.weak, &regs.alu.neg),
		signal_init("lub", &regs.alu.weja, &regs.alu.weak, &regs.alu.lub),
		signal_init("i", &regs.alu.weja, &regs.alu.weak, &regs.alu.i),
		signal_init("wyak", &regs.alu.ak, &regs.addr.mag_s),

		// MEM
		signal_init("wea", &regs.addr.mag_a, &regs.mem.reg_a),
		signal_init("wys", &regs.mem.reg_s, &regs.addr.mag_s),
		signal_init("wes", &regs.addr.mag_s, &regs.mem.reg_s),
		signal_init("czyt", &regs.mem.czyt, &regs.mem.reg_s),
		signal_init("pisz", &regs.mem.reg_s, &regs.mem.pisz),

		// ADDR
		signal_init("as", &regs.addr.mag_a, &regs.addr.mag_a),
		//signal_init("sa", &regs.addr.mag_a, &regs.addr.mag_a),
		signal_init("wel", &regs.addr.mag_a, &regs.addr.l),
		signal_init("wyl", &regs.addr.l, &regs.addr.mag_a),
		signal_init("wyls", &regs.addr.l, &regs.addr.mag_s),
		signal_init("wei", &regs.addr.mag_s, &regs.addr.i),
		signal_init("wyad", &regs.addr.i, &regs.addr.mag_a),
		signal_init("il", &regs.addr.il, &regs.addr.l),

		// XY
		signal_init("wex", &regs.mem.reg_s, &regs.xy.x),
		signal_init("wyx", &regs.xy.x, &regs.mem.reg_s),
		signal_init("wey", &regs.mem.reg_s, &regs.xy.y),
		signal_init("wyy", &regs.xy.y, &regs.mem.reg_s),

		// STACK
		signal_init("wews", &regs.addr.mag_a, &regs.stack.ws),
		signal_init("wyws", &regs.stack.ws, &regs.addr.mag_a),
		signal_init("iws", &regs.stack.iws, &regs.stack.ws),
		signal_init("dws", &regs.stack.dws, &regs.stack.ws),

		// IO
		// TODO


		// INTRP
		signal_init("werm", &regs.addr.mag_a, &regs.intrp.rm),
		signal_init("wyrm", &regs.intrp.rm, &regs.addr.mag_a),
		signal_init("wyap", &regs.intrp.ap, &regs.addr.mag_a),
	};
}
*/

Error cpu_init(struct CPU** cpu_ptr)
{
	if (!cpu_ptr)
		return ERROR_NULL_POINTER;

	struct CPU* cpu = malloc(sizeof(struct CPU));
	if (!cpu)
		return ERROR_MALLOC_FAILURE;

	setup_init(&cpu->setup);
	Error error = NO_ERROR;
	

	// TODO: usun¹æ
	struct Signal signals[] = {
		{"weja", NULL, NULL},
		{"shr", NULL, NULL},
		{"dziel", NULL, NULL},
		{"mno", NULL, NULL},
		{"przep", NULL, NULL},
		{"ode", NULL, NULL},
		{"dod", NULL, NULL},
		{"weak", NULL, NULL},
		{"iak", NULL, NULL},
		{"dak", NULL, NULL},
		{"neg", NULL, NULL},
		{"lub", NULL, NULL},
		{"i", NULL, NULL},
		{"wyak", NULL, NULL},
		{"wea", NULL, NULL},
		{"czyt", NULL, NULL},
		{"pisz", NULL, NULL},
		{"wys", NULL, NULL},
		{"wes", NULL, NULL},
		{"start", NULL, NULL},
		{"wyg", NULL, NULL},
		{"werb", NULL, NULL},
		{"wyrb", NULL, NULL},
		{"wey", NULL, NULL},
		{"wyy", NULL, NULL},
		{"wex", NULL, NULL},
		{"wyx", NULL, NULL},
		{"wei", NULL, NULL},
		{"as", NULL, NULL},
		{"wyad", NULL, NULL},
		{"stop", NULL, NULL},
		{"wyls", NULL, NULL},
		{"il", NULL, NULL},
		{"wyl", NULL, NULL},
		{"wel", NULL, NULL},
		{"wyrm", NULL, NULL},
		{"werm", NULL, NULL},
		{"wyap", NULL, NULL},
		{"rint", NULL, NULL},
		{"eni", NULL, NULL},
		{"wyws", NULL, NULL},
		{"wews", NULL, NULL},
		{"iws", NULL, NULL},
		{"dws", NULL, NULL}
	};

	cpu->signals = malloc(sizeof(signals));
	if (!cpu->signals)
		ERROR_MALLOC_FAILURE;

	*cpu_ptr = cpu;
	return error;
}

Error cpu_run(struct CPU* cpu)
{
	if (!cpu)
		return ERROR_NULL_POINTER;

	const char file_name[] = "MaszynaW.lst";
	Error error = compile_instructions(file_name, cpu);

	return error;
}

struct CPUSetup* cpu_get_setup(struct CPU* cpu)
{
	return cpu ? &cpu->setup : NULL;
}

struct CPUTags* cpu_get_tags(struct CPU* cpu)
{
	return cpu ? &cpu->tags : NULL;
}

struct Signal* cpu_get_signals(struct CPU* cpu)
{
	return cpu ? &cpu->tags : NULL;
}

void cpu_delete(struct CPU* cpu)
{
	free(cpu);
}