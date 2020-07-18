#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

/** Makro s³u¿¹ce do zg³aszania krytycznych b³êdów aplikacji. */
#define CRASH_LOG(x) do {_crash_log(__FILE__, __func__, __LINE__, x); exit(-1);} while(0)
/** Makro s³u¿¹ce do do sprawdzania czy podany wskaŸnik ma wartoœæ 'NULL'. Jeœli tak, aplikacja koñczy dzia³anie. */
#define CHECK_IF_NULL(x) do {if (!(x)) { _crash_log(__FILE__, __func__, __LINE__, NULL_DEREFERENCE); exit(-1); }} while(0)

/** Typ wyliczeniowy reprezentuj¹cy krytyczne b³êdy dzia³ania aplikacji. */
typedef enum { 
	NULL_DEREFERENCE,		///< wy³uskanie wskaŸnika na 'NULL'
	LIBRARY_FUNC_FAILURE,	///< b³¹d zg³oszony przez funkcjê biblioteczn¹
	MEM_ALOC_FAILURE, 		///< b³¹d alokacji pamiêci
	LOG_UNKNOWN_VALUE, 		///< wartoœæ, która nie powinna siê pojawiæ w danym miejscu
	GRAPHICS_FAILURE, 		///< b³¹d obs³ugi grafiki
	TERMINAL_FAILURE 		///< b³¹d obs³ugi terminala
} ProgramErrorType;

/** Funkcja inicjalizuj¹ca system zg³aszania krytycznych b³êdów. */
void init_crash_log();
/** Funkcja zapisuj¹ca informacje o napotkanym krytycznym b³êdzie.
	Zapisywane jest miejsce wyst¹pienia oraz typ.
@param file nazwa pliku
@param func funkcja
@param line linia
@param type typ b³êdu */
void _crash_log(const char* file, const char* func, int line, ProgramErrorType type);

/** Typ s³u¿¹cy do przechowywania b³êdów. */
typedef int Error;

/** Rodzaje niekrytycznych b³êdów spowodowanych przez u¿ytkownika. */
typedef Error UserErrorType;
#define USER_ERROR_TYPE  0xE000	///< ogólnie, dowolny b³¹d u¿ytkownika
#define INSTR_COMP_ERROR 0x8000	///< b³¹d kompilacji pliku instrukcji
#define PROG_COMP_ERROR	 0x4000	///< b³¹d kompilacji pliku programu
#define RUNTIME_ERROR	 0x2000	///< b³¹d wykonywania programu

/** B³êdy kompilacji. */
typedef Error CompilationError;
#define NO_FILE				0x0001	///< b³¹d odczytu pliku
#define MISSING_INPUT		0x0002	///< brak oczekiwanego elementu
#define INVALID_INPUT		0x0004	///< niepoprawny element
#define MISSING_LINE		0x0008	///< brakuj¹ca linia
#define UNKNOWN_LABEL		0x0010	///< nieznana etykieta
#define REPEATED_LABEL		0x0020	///< powtórzona etykieta
#define LOST_TICK			0x0040	///< takt instrukcji nigdy nie zostanie wykonany
#define FULL_MEMORY			0x0080	///< pe³na pamiêæ programu

/** B³êdy wykonywania programu. */
typedef Error RuntimeError;
#define CPU_STOPPED			0x0001	///< ustawiony zosta³ sygna³ 'stp'
#define UNKNOWN_INSTRUCTION	0x0002	///< napotkano instrukcjê o nieznaym kodzie
#define ALREADY_SET			0x0004	///< dany uk³ad jest u¿ywany (wartoœæ ju¿ zapisana)
#define EMPTY_UNIT			0x0008	///< dany uk³ad jest pusty (brak wartoœci)
#define INVALID_IO_ADDRESS	0x0010	///< niepoprawny adres urz¹dzenia we/wy

/** Funkcja zwracaj¹ca typ zapisanego b³êdu. 
@return typ b³edu */
Error error();
/** Funkcja zwracaj¹ca informacjê tekstow¹ na temat zapisanego b³êdu.
@return informacja tekstowa */
const char* error_msg();
/** Funkcja zapisuj¹ca b³¹d kompilacji pliku instrukcji.
@param error typ b³êdu kompilacji
@param arg dodatkowa informacja tekstowa (opcjonalne) */
void instr_error_set(CompilationError error, const char* arg);
/** Funkcja zapisuj¹ca b³¹d kompilacji pliku programu.
@param error typ b³êdu kompilacji
@param arg dodatkowa informacja tekstowa (opcjonalne) */
void prog_error_set(CompilationError error, const char* arg);
/** Funkcja zapisuj¹ca b³¹d wykonywania programu.
@param error typ b³êdu programu
@param arg dodatkowa informacja tekstowa (opcjonalne) */
void runtime_error_set(RuntimeError error, const char* arg);
/** Funkcja kasuj¹ca zapisany b³¹d. */
void error_reset();

/** Bezpieczna funkcja malloc. W razie niepowodzenia alokacji pamiêci koñczy pracê aplikacji. 
@param size rozmiar pamiêci do zaalokowania
@return wskaŸnik na zaalokowan¹ pamiêæ */
void* malloc_s(size_t size);
/** Bezpieczna funkcja calloc. W razie niepowodzenia alokacji pamiêci koñczy pracê aplikacji. 
@param count liczba obiektów nowej tablicy
@param size rozmiar obiektu
@return wskaŸnik na zaalokowan¹ pamiêæ */
void* calloc_s(size_t count, size_t size);
/** Bezpieczna funkcja realloc. W razie niepowodzenia alokacji pamiêci koñczy pracê aplikacji. 
@param ptr wskaŸnik na poprzednio zaalokowan¹ pamiêæ
@param size nowy rozmiar pamiêci do zaalokowania
@return wskaŸnik na zaalokowan¹ pamiêæ */
void* realloc_s(void* ptr, size_t new_size);

#endif