#ifndef ERROR_H
#define ERROR_H
// Struktura zajmuj�ca si� obs�ug� b��d�w
// - jedna na ca�y program
// - ko�czy prac� programu w razie wykrycia b��du
// - komunikat?

#define NO_ERROR 0
#define ERROR -1
#define ERROR_TERMINAL_FAILURE		0x0001
#define ERROR_EVENTS_FAILURE		0x0002
#define ERROR_WINDOW_FAILURE		0x0004
#define ERROR_RENDER_FAILURE		0x0008
#define ERROR_SIGNAL_FAILURE		0x0010
#define ERROR_NO_INSTR_FILE			0x0020
#define ERROR_INVALID_INSTR_FILE	0x0040
#define ERROR_STRING_HANDLING		0x0080
#define ERROR_INSTR_COMPILATION		0x0100
#define ERROR_UNIT_ALREADY_SET		0x0200		
#define ERROR_INSTR_ADDR_OUT_OF_RANGE	0x0400		
#define ERROR_CPU_STOPPED			0x0800
#define ERROR_EMPTY_UNIT			0x1000
#define ERROR_NO_PROGRAM_FILE		0x2000
#define ERROR_INVALID_PROGRAM_FILE	0x4000
#define ERROR_MEMORY_TOO_SHORT		0x8000

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

Error error_get();
void error_set(Error error);
void error_set_msg(Error error, const char* message);
void critical_error_set(const char* message);
const char* error_get_msg();
void* malloc_s(size_t size);
void* calloc_s(size_t count, size_t size);
void* realloc_s(void* ptr, size_t new_size);
void check_for_NULL(const void* ptr);

void debug_error_delete();

#endif