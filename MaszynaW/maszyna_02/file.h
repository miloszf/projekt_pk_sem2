#ifndef FILE_H
#define FILE_H

#include <stdbool.h>
#include "settings.h"

/** Pomocnicza struktura przechowuj¹ca dane wczytane z pliku. */
struct FileHandler;

/** Funkcja inicjalizuj¹ca obiekt struktury FileHandler.
@return nowy obiekt */
struct FileHandler* file_handler_init();
/** Funkcja usuwaj¹ca podany obiekt
@param handler obiekt do usuniêcia */
void file_handler_delete(struct FileHandler* handler);

/** Funkcja wczytuj¹ca ustawienia symulatora z listy rozkazów.
@param file_name nazwa pliku instrukcji
@param handler struktura przechowuj¹c¹ dane wczytane z pliku
@param pref_map mapa ustawieñ
@return 'true' przy braku b³êdów, 'false' w razie niepowodzenia */
bool file_import_setup(const char* file_name, struct FileHandler* handler, struct Map* pref_map);
/** Funkcja kompiluj¹ca listê rozkazów.
@param handler struktura przechowuj¹ca dane wczytane z pliku
@param pref_map mapa sygna³ów
@param pref_map mapa tagów
@return wektor instrukcji lub NULL w razie niepowodzenia */
struct Vector* file_compile_instructions(struct FileHandler* handler, struct Map* signal_map, struct Map* tag_map);
/** Funkcja kompiluj¹ca program, wype³nia pamiêæ maszyny wartoœciami odpowiadaj¹cymi danym instrukcjom.
@param file_name nazwa pliku programu
@param instr_vect wektor instrukcji
@param addr_len liczba bitów adresowych
@param code_len liczba bitów kodu
@param memory pamiêæ programu
@return 'true' przy braku b³êdów, 'false' w razie niepowodzenia */
bool file_compile_program(const char* file_name, struct Vector* instr_vect, var addr_len, var code_len, var* memory);

#endif