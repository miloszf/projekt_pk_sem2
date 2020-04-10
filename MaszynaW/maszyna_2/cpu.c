#include "cpu.h"
#include "error.h"
#include "reg.h"
#include "reg_functions.h"
#include "signal.h"


//typedef struct Reg sig;
//



struct CPU
{
	

};

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

struct CPU* cpu_init(struct CPUSetup* cu_setup)
{
	struct CPU* cu = malloc(sizeof(struct CPU));

	if (cu && cu_setup)
	{

	}

	return cu;
}

void cpu_delete(struct CPU* cpu)
{
	free(cpu);
}