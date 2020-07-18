#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

/** Makro s�u��ce do zg�aszania krytycznych b��d�w aplikacji. */
#define CRASH_LOG(x) do {_crash_log(__FILE__, __func__, __LINE__, x); exit(-1);} while(0)
/** Makro s�u��ce do do sprawdzania czy podany wska�nik ma warto�� 'NULL'. Je�li tak, aplikacja ko�czy dzia�anie. */
#define CHECK_IF_NULL(x) do {if (!(x)) { _crash_log(__FILE__, __func__, __LINE__, NULL_DEREFERENCE); exit(-1); }} while(0)

/** Typ wyliczeniowy reprezentuj�cy krytyczne b��dy dzia�ania aplikacji. */
typedef enum { 
	NULL_DEREFERENCE,		///< wy�uskanie wska�nika na 'NULL'
	LIBRARY_FUNC_FAILURE,	///< b��d zg�oszony przez funkcj� biblioteczn�
	MEM_ALOC_FAILURE, 		///< b��d alokacji pami�ci
	LOG_UNKNOWN_VALUE, 		///< warto��, kt�ra nie powinna si� pojawi� w danym miejscu
	GRAPHICS_FAILURE, 		///< b��d obs�ugi grafiki
	TERMINAL_FAILURE 		///< b��d obs�ugi terminala
} ProgramErrorType;

/** Funkcja inicjalizuj�ca system zg�aszania krytycznych b��d�w. */
void init_crash_log();
/** Funkcja zapisuj�ca informacje o napotkanym krytycznym b��dzie.
	Zapisywane jest miejsce wyst�pienia oraz typ.
@param file nazwa pliku
@param func funkcja
@param line linia
@param type typ b��du */
void _crash_log(const char* file, const char* func, int line, ProgramErrorType type);

/** Typ s�u��cy do przechowywania b��d�w. */
typedef int Error;

/** Rodzaje niekrytycznych b��d�w spowodowanych przez u�ytkownika. */
typedef Error UserErrorType;
#define USER_ERROR_TYPE  0xE000	///< og�lnie, dowolny b��d u�ytkownika
#define INSTR_COMP_ERROR 0x8000	///< b��d kompilacji pliku instrukcji
#define PROG_COMP_ERROR	 0x4000	///< b��d kompilacji pliku programu
#define RUNTIME_ERROR	 0x2000	///< b��d wykonywania programu

/** B��dy kompilacji. */
typedef Error CompilationError;
#define NO_FILE				0x0001	///< b��d odczytu pliku
#define MISSING_INPUT		0x0002	///< brak oczekiwanego elementu
#define INVALID_INPUT		0x0004	///< niepoprawny element
#define MISSING_LINE		0x0008	///< brakuj�ca linia
#define UNKNOWN_LABEL		0x0010	///< nieznana etykieta
#define REPEATED_LABEL		0x0020	///< powt�rzona etykieta
#define LOST_TICK			0x0040	///< takt instrukcji nigdy nie zostanie wykonany
#define FULL_MEMORY			0x0080	///< pe�na pami�� programu

/** B��dy wykonywania programu. */
typedef Error RuntimeError;
#define CPU_STOPPED			0x0001	///< ustawiony zosta� sygna� 'stp'
#define UNKNOWN_INSTRUCTION	0x0002	///< napotkano instrukcj� o nieznaym kodzie
#define ALREADY_SET			0x0004	///< dany uk�ad jest u�ywany (warto�� ju� zapisana)
#define EMPTY_UNIT			0x0008	///< dany uk�ad jest pusty (brak warto�ci)
#define INVALID_IO_ADDRESS	0x0010	///< niepoprawny adres urz�dzenia we/wy

/** Funkcja zwracaj�ca typ zapisanego b��du. 
@return typ b�edu */
Error error();
/** Funkcja zwracaj�ca informacj� tekstow� na temat zapisanego b��du.
@return informacja tekstowa */
const char* error_msg();
/** Funkcja zapisuj�ca b��d kompilacji pliku instrukcji.
@param error typ b��du kompilacji
@param arg dodatkowa informacja tekstowa (opcjonalne) */
void instr_error_set(CompilationError error, const char* arg);
/** Funkcja zapisuj�ca b��d kompilacji pliku programu.
@param error typ b��du kompilacji
@param arg dodatkowa informacja tekstowa (opcjonalne) */
void prog_error_set(CompilationError error, const char* arg);
/** Funkcja zapisuj�ca b��d wykonywania programu.
@param error typ b��du programu
@param arg dodatkowa informacja tekstowa (opcjonalne) */
void runtime_error_set(RuntimeError error, const char* arg);
/** Funkcja kasuj�ca zapisany b��d. */
void error_reset();

/** Bezpieczna funkcja malloc. W razie niepowodzenia alokacji pami�ci ko�czy prac� aplikacji. 
@param size rozmiar pami�ci do zaalokowania
@return wska�nik na zaalokowan� pami�� */
void* malloc_s(size_t size);
/** Bezpieczna funkcja calloc. W razie niepowodzenia alokacji pami�ci ko�czy prac� aplikacji. 
@param count liczba obiekt�w nowej tablicy
@param size rozmiar obiektu
@return wska�nik na zaalokowan� pami�� */
void* calloc_s(size_t count, size_t size);
/** Bezpieczna funkcja realloc. W razie niepowodzenia alokacji pami�ci ko�czy prac� aplikacji. 
@param ptr wska�nik na poprzednio zaalokowan� pami��
@param size nowy rozmiar pami�ci do zaalokowania
@return wska�nik na zaalokowan� pami�� */
void* realloc_s(void* ptr, size_t new_size);

#endif