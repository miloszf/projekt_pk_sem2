#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include "settings.h"
// "Biblioteka" obs�ugi plik�w

// Funkcja wczytuj�ca ustawienia maszynyW z listy rozkaz�w, zwraca 0 w przypadku niepowodzenia
// argumenty: mapa ustawie�
bool files_import_setup(const char* file_name, struct Map* pref_map);

// Funkcja kompiluj�ca list� rozkaz�w, zwraca 0 w przypadku niepowodzenia
// zwraca wektor wczytanych instrukcji
// argumenty: nazwa pliku, mapa sygna��w, mapa znacznik�w
bool files_compile_instructions(const char* file_name, struct Map* signal_map, struct Map* tag_map, struct Vector* instr_vect);

// Funkcja kompiluj�ca program, wype�nia pami�� maszyny, zwraca 0 w przypadku niepowodzenia
// wczytuje do program do podanej pami�ci 
// argumenty: nazwa pliku, mapa rozkaz�w, d�ugo�� s�owa maszyny
bool files_compile_program(const char* file_name, struct Map* instr_map, unsigned char addr_len, unsigned char code_len, var* memory);

#endif