#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include "settings.h"

/** Struktura reprezentuj�ca jednostk� centraln� maszyny. */
struct CPU;

/** Funkcja inicjalizuj�ca obiekt struktury Window.
@param canvas scena w kt�rej zawiera si� jednostka centralna
@return nowy obiekt */
struct CPU* cpu_init(struct Canvas* canvas);
/** Funkcja wczytuj�ca i kompiluj�ca list� rozkaz�w.
@param cpu jednostka centralna
@param file_name nazwa pliku
@return powodzenie kompilacji pliku */
bool cpu_import_instructions(struct CPU* cpu, const char* file_name);
/** Funkcja wczytuj�ca i kompiluj�ca program.
@param cpu jednostka centralna
@param file_name nazwa pliku
@return powodzenie kompilacji pliku */
bool cpu_import_program(struct CPU* cpu, const char* file_name);
/** Funkcja wykonuj�ca jeden takt, zwraca wska�nik na nast�pny takt lub NULL gdy w�asnie wykonany 
	takt by� ostatni w danym rozkazie lub w przypadku b��du.
@param cpu jednostka centralna
@return wska�nik na nast�pny takt lub 'NULL' dla ostaniego taktu lub b��du */
void* cpu_tick(struct CPU* cpu);
/** Funkcja zwracaj�ca flagi gotowo�wi wej�cia/wyj�cia maszyny.
@param cpu jednostka centralna
@param input_flag wska�nik na flag� gotowo�ci do pobrania znaku
@param output_flag wska�nik na flag� gotowo�ci do wys�ania znaku */
void cpu_get_io_flags(struct CPU* cpu, bool* input_flag, bool* output_flag);
/** Funkcja obs�uguj�ca interakcj� z u�ytkownikiem.
@param cpu jednostka centralna
@param mouse_scroll przesuni�cie pami�ci
@param interrupts zg�oszone przerwania
@param input_char wczytany znak
@param output_char wska�nik na znak do zapisania */
void cpu_user_input(struct CPU* cpu, var mouse_scroll, var interrupts, char input_char, char* output_char);
/** Funkcja resetuj�ca stan maszyny, z wy��czeniem pami�ci
@param cpu jednostka centralna */
void cpu_reset(struct CPU* cpu);
/** Funkcja usuwaj�ca podany obiekt
@param window obiekt do usuni�cia */
void cpu_delete(struct CPU* cpu);

#endif