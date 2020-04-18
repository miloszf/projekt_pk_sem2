#ifndef SIGNAL_H
#define SIGNAL_H

#include "error.h"

//struct Signal;
struct Signal
{
	const unsigned char* name;
	void* value;
	void* (*fun)(struct Signal*);
	//bool is_set;
};

//struct Signal* signal_init(char* name, struct Reg* in, struct Reg* out, struct Reg* through);
//struct Signal* signal_init(char* name, struct Reg* in, struct Reg* out);
//Error signal_set(struct Signal* sig);

#endif
