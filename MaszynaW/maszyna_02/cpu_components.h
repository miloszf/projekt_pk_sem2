#ifndef CPU_COMPONENTS_H
#define CPU_COMPONENTS_H

#include "settings.h"

struct CPUTag cpu_tag_init(const char* name);
void cpu_tag_delete(struct CPUTag* tag);

void cpu_init_alu(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_mem(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_addr(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_xy(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_stack(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_io(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_intr(struct CPU* cpu, const Point offset, struct Canvas* canvas);

#endif
