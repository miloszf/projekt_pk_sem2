#ifndef CPU_COMPONENTS_H
#define CPU_COMPONENTS_H

#include "settings.h"

/** Funkcja inicjalizuj¹ca uk³ady jednostki arytmetyczno-logicznej maszyny.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_alu_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca sygna³y jednostki arytmetyczno-logicznej maszyny.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_alu_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca uk³ady pamiêci maszyny.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_mem_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca sygna³y pamiêci maszyny.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_mem_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca sygna³y pamiêci maszyny.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_addr_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca uk³ady zwi¹zane z przetwarzaniem adresu.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_addr_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca sygna³y zwi¹zane z przetwarzaniem adresu.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_xy_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca rejstry X i Y.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_xy_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca sygna³y zwi¹zane z obs³ug¹ rejstrów X i Y.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_stack_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca rejstry obs³uguj¹ce stos.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_stack_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca sygna³y zwi¹zane z obs³ug¹ stosu.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_io_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca rejstry obs³uguj¹ce rejstry we/wy.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_io_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca sygna³y zwi¹zane z obs³ug¹ we/wy.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_intr_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca rejstry obs³uguj¹ce rejstry przerwañ.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
void cpu_init_intr_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj¹ca sygna³y zwi¹zane z obs³ug¹ przerwañ.
@param cpu jednostka centralna
@param offset pozycja wzglêdem lewego górnego rogu sceny
@param canvas scena */
#endif
