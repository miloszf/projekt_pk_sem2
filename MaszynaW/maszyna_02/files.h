#ifndef FILES_H
#define FILES_H

#include <stdbool.h>
#include "settings.h"
// "Biblioteka" obs³ugi plików

// Funkcja wczytuj¹ca ustawienia maszynyW z listy rozkazów, zwraca 0 w przypadku niepowodzenia
// argumenty: mapa ustawieñ
bool files_import_setup(const char* file_name, struct Map* pref_map);

// Funkcja kompiluj¹ca listê rozkazów, zwraca 0 w przypadku niepowodzenia
// zwraca wektor wczytanych instrukcji
// argumenty: nazwa pliku, mapa sygna³ów, mapa znaczników
bool files_compile_instructions(const char* file_name, struct Map* signal_map, struct Map* tag_map, struct Vector* instr_vect);

// Funkcja kompiluj¹ca program, wype³nia pamiêæ maszyny, zwraca 0 w przypadku niepowodzenia
// wczytuje do program do podanej pamiêci 
// argumenty: nazwa pliku, mapa rozkazów, d³ugoœæ s³owa maszyny
bool files_compile_program(const char* file_name, struct Map* instr_map, unsigned char addr_len, unsigned char code_len, var* memory);

#endif