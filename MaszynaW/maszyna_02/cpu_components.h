#ifndef CPU_COMPONENTS_H
#define CPU_COMPONENTS_H

#include "settings.h"

/** Funkcja inicjalizuj�ca uk�ady jednostki arytmetyczno-logicznej maszyny.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_alu_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca sygna�y jednostki arytmetyczno-logicznej maszyny.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_alu_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca uk�ady pami�ci maszyny.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_mem_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca sygna�y pami�ci maszyny.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_mem_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca sygna�y pami�ci maszyny.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_addr_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca uk�ady zwi�zane z przetwarzaniem adresu.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_addr_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca sygna�y zwi�zane z przetwarzaniem adresu.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_xy_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca rejstry X i Y.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_xy_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca sygna�y zwi�zane z obs�ug� rejstr�w X i Y.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_stack_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca rejstry obs�uguj�ce stos.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_stack_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca sygna�y zwi�zane z obs�ug� stosu.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_io_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca rejstry obs�uguj�ce rejstry we/wy.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_io_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca sygna�y zwi�zane z obs�ug� we/wy.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_intr_units(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca rejstry obs�uguj�ce rejstry przerwa�.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
void cpu_init_intr_signals(struct CPU* cpu, const Point offset, struct Canvas* canvas);
/** Funkcja inicjalizuj�ca sygna�y zwi�zane z obs�ug� przerwa�.
@param cpu jednostka centralna
@param offset pozycja wzgl�dem lewego g�rnego rogu sceny
@param canvas scena */
#endif
