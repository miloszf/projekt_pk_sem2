#ifndef CPU_COMPONENTS_H
#define CPU_COMPONENTS_H

#include "settings.h"

void cpu_init_alu_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_alu_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_mem_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_mem_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_addr_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_addr_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_xy_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_xy_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_stack_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_stack_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_io_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_io_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_intr_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
void cpu_init_intr_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);

#endif
