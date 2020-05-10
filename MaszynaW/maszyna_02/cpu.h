#ifndef CPU_H
#define CPU_H

// Struktura b�d�ca "sercem" maszynyW. Inicjalizuje bloki, sygna�y, wczytuje rozkazy i program.
// Wykonuje takty lub rozkazy. Po�redniczy mi�dzy sygna�ami zewn�trznymi (przerwania, we/wy klawiatury)

struct CPU;

// Funkcja tworz�ca now� jednostk� centraln�
// Nowa jednostka jest ju� wst�pnie zainicjalizowana, tj. utworzone s� bloki, sygna�y,
// odpowiednie struktury i po��czenia
// argumenty: [?], bufor znak�w z klawiatury do odczytania, bufor znak�w do zapisania
// zwraca: wska�nik na now� jednostk�
struct CPU* cpu_init(struct Canvas* canvas);

// Funkcja wczytuj�ca i kompiluj�ca list� rozkaz�w i ustawie�
// argumenty: nazwa pliku z rozkazami
bool cpu_import_instructions(struct CPU* cpu, const char* file_name);

// Funkcja wczytuj�ca i kompiluj�ca program. 
// argumenty: nazwa pliku z programem
bool cpu_import_program(struct CPU* cpu, const char* file_name);

// Funkcja wykonuj�ca jeden takt, zwraca wska�nik na nast�pny takt lub NULL gdy w�asnie wykonany 
// takt by� ostatni w danym rozkazie lub w przypadku b��du
// argumenty: [?]
void* cpu_tick(struct CPU* cpu);

// Funkcja wykonuj�ca jeden rozkaz
// NIE MA!!! (mo�liwa blokada)

// Funkcja zapisuj�ca zg�oszone przerwanie do maszyny
// argumenty: przerwania do zapisania

// Funkcja resetuj�ca maszyn� (bez pami�ci)
// argumenty: -


void cpu_delete(struct CPU* cpu);

#endif