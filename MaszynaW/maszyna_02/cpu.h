#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include "settings.h"

// Struktura bêd¹ca "sercem" maszynyW. Inicjalizuje bloki, sygna³y, wczytuje rozkazy i program.
// Wykonuje takty lub rozkazy. Poœredniczy miêdzy sygna³ami zewnêtrznymi (przerwania, we/wy klawiatury)
struct CPU;

// Funkcja tworz¹ca now¹ jednostkê centraln¹
// Nowa jednostka jest ju¿ wstêpnie zainicjalizowana, tj. utworzone s¹ bloki, sygna³y,
// odpowiednie struktury i po³¹czenia
// argumenty: [?], bufor znaków z klawiatury do odczytania, bufor znaków do zapisania
// zwraca: wskaŸnik na nowê jednostkê
struct CPU* cpu_init(struct Canvas* canvas);

// Funkcja wczytuj¹ca i kompiluj¹ca listê rozkazów i ustawieñ
// argumenty: nazwa pliku z rozkazami
bool cpu_import_instructions(struct CPU* cpu, const char* file_name);

// Funkcja wczytuj¹ca i kompiluj¹ca program. 
// argumenty: nazwa pliku z programem
bool cpu_import_program(struct CPU* cpu, const char* file_name);

// Funkcja wykonuj¹ca jeden takt, zwraca wskaŸnik na nastêpny takt lub NULL gdy w³asnie wykonany 
// takt by³ ostatni w danym rozkazie lub w przypadku b³êdu
// argumenty: [?]
void* cpu_tick(struct CPU* cpu);

void cpu_get_io_flags(struct CPU* cpu, bool* input_flag, bool* output_flag);

void cpu_user_input(struct CPU* cpu, var mouse_scroll, var interrupts, char input_char, char* output_char);

//void cpu_get_char(struct CPU* cpu);

//void cpu_set_char(struct CPU* cpu);

// Funkcja resetuj¹ca maszynê (bez pamiêci)
// argumenty: -
void cpu_reset(struct CPU* cpu);

void cpu_delete(struct CPU* cpu);

#endif