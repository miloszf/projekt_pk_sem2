#ifndef FILE_H
#define FILE_H

#include <stdbool.h>
#include "settings.h"
// "Biblioteka" obs³ugi plików

struct FileHandler;

struct FileHandler* file_handler_init();
void file_handler_delete(struct FileHandler* handler);

// Funkcja wczytuj¹ca ustawienia maszynyW z listy rozkazów, zwraca 0 w przypadku niepowodzenia
// argumenty: mapa ustawieñ
bool file_import_setup(const char* file_name, struct FileHandler* files_handler, struct Map* pref_map);

// Funkcja kompiluj¹ca listê rozkazów, zwraca 0 w przypadku niepowodzenia
// zwraca wektor wczytanych instrukcji
// argumenty: nazwa pliku, mapa sygna³ów, mapa znaczników
struct Vector* file_compile_instructions(struct FileHandler* handler, struct Map* signal_map, struct Map* tag_map);

// Funkcja kompiluj¹ca program, wype³nia pamiêæ maszyny, zwraca 0 w przypadku niepowodzenia
// wczytuje do program do podanej pamiêci 
// argumenty: nazwa pliku, mapa rozkazów, d³ugoœæ s³owa maszyny
bool file_compile_program(const char* file_name, struct Map* instr_map, unsigned char addr_len, unsigned char code_len, var* memory);

#endif