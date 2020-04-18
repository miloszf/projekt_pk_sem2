#ifndef SIMULATOR_H
#define SIMULATOR_H

struct Simulator;

struct Simulator* sim_init(int argc, const char** argv);
void sim_run(struct Simulator* sim);
void sim_delete(struct Simulator* sim);

#endif