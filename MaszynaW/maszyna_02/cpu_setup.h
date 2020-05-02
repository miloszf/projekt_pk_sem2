#ifndef CPU_SETUP_H
#define CPU_SETUP_H

struct CPUPreference cpu_preference_init(const char* name);
void cpu_preference_delete(struct CPUPreference* setup);

struct CPUSetup cpu_setup_init();
void cpu_setup_delete(struct CPUSetup* setup);

#endif