#ifndef FILE_H
#define FILE_H

#include <stdbool.h>
#include "settings.h"

/** Pomocnicza struktura przechowuj�ca dane wczytane z pliku. **/
struct FileHandler;

/** Funkcja inicjalizuj�ca obiekt struktury FileHandler.
@return nowy obiekt */
struct FileHandler* file_handler_init();
/** Funkcja usuwaj�ca podany obiekt
@param handler obiekt do usuni�cia */
void file_handler_delete(struct FileHandler* handler);

/** Funkcja wczytuj�ca ustawienia symulatora z listy rozkaz�w.
@param file_name nazwa pliku
@param handler struktura przechowuj�c� dane wczytane z pliku
@param pref_map  mapa ustawie�
@return 'true' przy braku b��d�w, 'false' w przypadku niepowodzenia */
bool file_import_setup(const char* file_name, struct FileHandler* handler, struct Map* pref_map);
/** Funkcja kompiluj�ca list� rozkaz�w.
@param handler struktura przechowuj�ca dane wczytane z pliku
@param pref_map mapa sygna��w
@param pref_map mapa tag�w
@return wektor instrukcji */
struct Vector* file_compile_instructions(struct FileHandler* handler, struct Map* signal_map, struct Map* tag_map);
/** Funkcja kompiluj�ca program, wype�nia pami�� maszyny warto�ciami odpowiadaj�cymi
	danym instrukcjom.
@param file_name nazwa pliku
@param instr_vect wektor instrukcji
@param addr_len liczba bit�w adresowych
@param code_len liczba bit�w kodu
@param memory pami�� programu
@return 'true' przy braku b��d�w, 'false' w przypadku niepowodzenia */
bool file_compile_program(const char* file_name, struct Vector* instr_vect, var addr_len, var code_len, var* memory);

#endif