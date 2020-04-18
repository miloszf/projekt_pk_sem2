#ifndef REG_H
#define REG_H

#include "settings.h"
#include "error.h"

typedef Error (*RegFun)(struct Reg*);

struct Reg;

//struct Reg* reg_init(RegFun set, RegFun reset);
//struct Reg* reg_init(RegFun set, RegFun reset, var(*get_value)(var, var));
struct Reg* reg_init(RegFun set, RegFun reset, var(*get_value)(var, var), struct Reg* input_ptr);



Error reg_set(struct Reg* reg);
Error comb_set(struct Reg* reg);
Error reg_reset(struct Reg* reg);
Error comb_reset(struct Reg* reg);

#endif