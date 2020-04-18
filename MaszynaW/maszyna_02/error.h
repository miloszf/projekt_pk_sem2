#ifndef ERROR_H
#define ERROR_H
// Struktura zajmuj�ca si� obs�ug� b��d�w
// - jedna na ca�y program
// - ko�czy prac� programu w razie wykrycia b��du
// - komunikat?

typedef int Error;
// b��dy krytyczne, wyst�pienie oznacza natychmiastowe zamkni�cie programu
// - b��d alokacji pami�ci
// - wy�uskanie wska�nika NULL
// - ?inne?

// b��dy niekrytyczne, program wy�wietla stosowny komunikat
// i czeka na reakcj� u�ytkownika:
// - przetwarzania podanych plik�w:
//   - b��d otwierania pliku
//   - nierozpoznane znaczniki
//   - niepoprawne warto�ci
// - b��d podczas wykonywania instrukcji/programu:
//   - nierozpoznana warto�� w rejestrze instrukcji

Error error();
void set_error(Error error, const char* message);
void* malloc_s(size_t size);
void* calloc_s(size_t count, size_t size);
void* realloc_s(void* ptr, size_t new_size);
void check_for_NULL(void* ptr);

#endif