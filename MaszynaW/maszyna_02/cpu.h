#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include "settings.h"

/** Struktura reprezentuj¹ca jednostkê centraln¹ maszyny. */
struct CPU;

/** Funkcja inicjalizuj¹ca obiekt struktury Window.
@param canvas scena w której zawiera siê jednostka centralna
@return nowy obiekt */
struct CPU* cpu_init(struct Canvas* canvas);
/** Funkcja wczytuj¹ca i kompiluj¹ca listê rozkazów.
@param cpu jednostka centralna
@param file_name nazwa pliku
@return powodzenie kompilacji pliku */
bool cpu_import_instructions(struct CPU* cpu, const char* file_name);
/** Funkcja wczytuj¹ca i kompiluj¹ca program.
@param cpu jednostka centralna
@param file_name nazwa pliku
@return powodzenie kompilacji pliku */
bool cpu_import_program(struct CPU* cpu, const char* file_name);
/** Funkcja wykonuj¹ca jeden takt, zwraca wskaŸnik na nastêpny takt lub NULL gdy w³asnie wykonany 
	takt by³ ostatni w danym rozkazie lub w przypadku b³êdu.
@param cpu jednostka centralna
@return wskaŸnik na nastêpny takt lub 'NULL' dla ostaniego taktu lub b³êdu */
void* cpu_tick(struct CPU* cpu);
/** Funkcja zwracaj¹ca flagi gotowoæwi wejœcia/wyjœcia maszyny.
@param cpu jednostka centralna
@param input_flag wskaŸnik na flagê gotowoœci do pobrania znaku
@param output_flag wskaŸnik na flagê gotowoœci do wys³ania znaku */
void cpu_get_io_flags(struct CPU* cpu, bool* input_flag, bool* output_flag);
/** Funkcja obs³uguj¹ca interakcjê z u¿ytkownikiem.
@param cpu jednostka centralna
@param mouse_scroll przesuniêcie pamiêci
@param interrupts zg³oszone przerwania
@param input_char wczytany znak
@param output_char wskaŸnik na znak do zapisania */
void cpu_user_input(struct CPU* cpu, var mouse_scroll, var interrupts, char input_char, char* output_char);
/** Funkcja resetuj¹ca stan maszyny, z wy³¹czeniem pamiêci
@param cpu jednostka centralna */
void cpu_reset(struct CPU* cpu);
/** Funkcja usuwaj¹ca podany obiekt
@param window obiekt do usuniêcia */
void cpu_delete(struct CPU* cpu);

#endif