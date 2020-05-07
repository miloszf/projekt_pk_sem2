#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdbool.h>

struct Simulator;

struct Simulator* simulator_init(int argc, char** argv);
void simulator_run(struct Simulator* sim);

#endif