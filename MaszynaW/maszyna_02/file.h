#ifndef FILE_H
#define FILE_H

#include <stdbool.h>
#include "settings.h"
// "Biblioteka" obs�ugi plik�w

struct FileHandler;

struct FileHandler* file_handler_init();
void file_handler_delete(struct FileHandler* handler);

// Funkcja wczytuj�ca ustawienia maszynyW z listy rozkaz�w, zwraca 0 w przypadku niepowodzenia
// argumenty: mapa ustawie�
bool file_import_setup(const char* file_name, struct FileHandler* files_handler, struct Map* pref_map);

// Funkcja kompiluj�ca list� rozkaz�w, zwraca 0 w przypadku niepowodzenia
// zwraca wektor wczytanych instrukcji
// argumenty: nazwa pliku, mapa sygna��w, mapa znacznik�w
struct Vector* file_compile_instructions(struct FileHandler* handler, struct Map* signal_map, struct Map* tag_map);

// Funkcja kompiluj�ca program, wype�nia pami�� maszyny, zwraca 0 w przypadku niepowodzenia
// wczytuje do program do podanej pami�ci 
// argumenty: nazwa pliku, mapa rozkaz�w, d�ugo�� s�owa maszyny
bool file_compile_program(const char* file_name, struct Vector* instr_vect, var addr_len, var code_len, var* memory);

#endif