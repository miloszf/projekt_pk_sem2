#ifndef CPU_FUNCTIONS_H
#define CPU_FUNCTIONS_H

#include <stdbool.h>
#include "settings.h"

/** Struktura przechowuj¹ca informacje potrzebne funkcji obs³uguj¹cej pamiêæ maszyny. */
struct SignalMemory
{
	var** memory;		///< wskaŸnik na pamiêæ
	struct Unit* reg_a;	///< rejestr zawieraj¹cy adres
	struct Unit* reg_s;	///< rejestr zawieraj¹cy s³owo
};
/** Struktura przechowuj¹ca informacje potrzebne funkcji obs³uguj¹cej przerwania. */
struct SignalInterrupts
{
	struct Unit* reg_rz;			///< rejestr zg³oszeñ
	struct Unit* reg_rm;			///< rejestr maski
	struct Unit* reg_rp;			///< rejestr przerwañ
	struct Unit* reg_ap;			///< rejestr adresów przerwañ
	bool* int_tag;					///< wskaŸnik na wartoœæ tagu przerwañ
	const var* intr_mask;			///< wskaŸnik na maskê przerwañ
	struct Drawable** buttons_array;///< wskaŸnik na tablicê prazycisków przerwañ
};
/** Struktura przechowuj¹ca informacje potrzebne funkcjom zwi¹zanych z obs³ug¹ instrukcji. */
struct SignalInstruction
{
	struct Unit* from;		///< uk³ad 'z'
	struct Unit* to;		///< uk³ad 'do'
	const var* addr_len;	///< iloœæ bitów adresowych
	const var* instr_num;	///< iloœæ instrukcji
};
/** Struktura przechowuj¹ca informacje potrzebne funkcjom obs³uguj¹cym po³¹czenia miêdzymagistralowe. */
struct SignalBusConnection
{
	struct Unit* bus_a;		///< pierwsza magistrala
	struct Unit* bus_b;		///< druga magistrala
	struct Unit* through;	///< poœrednia magistrala
	const var* mask;		///< wskaŸnik na maskê
};
/** Struktura przechowuj¹ca informacje potrzebne funkcjom obs³uguj¹cym operacje wejœcia/wyjœcia. */
struct SignalIOHandling
{
	bool* input_flag;			///< wskaŸnik na flagê gotowoœci do pobrania znaku 
	bool* output_flag;			///< wskaŸnik na flagê gotowoœci do zapisu znaku 
	struct Unit* address_reg;	///< rejestr instrukcji
	const var* addr_mask;		///< wskaŸnik na maskê adresow¹
};

/** Funkcja odczytuj¹ca dane z pamiêci.
@param value_ptr wskaŸnik na obiekt zawieraj¹cy potrzebne informacje. 
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var sig_read_from_memory(void* value_ptr);
/** Funkcja zapisuj¹ca dane do pamiêci.
@ param value_ptr wskaŸnik na obiekt zawieraj¹cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var sig_write_to_memory(void* value_ptr);
/** Funkcja odzczytuj¹ca zg³oszone przerwania.
@param value_ptr wskaŸnik na obiekt zawieraj¹cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var sig_enable_interrupts(void* value_ptr);
/** Funkcja resetuj¹ca rejsetry zwi¹zane z obs³u¿onym przerwaniem.
@param value_ptr wskaŸnik na obiekt zawieraj¹cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var sig_reset_interrupts(void* value_ptr);
/** Funkcja zatrzumuj¹ca pracê maszyny.
@param value_ptr wskaŸnik na obiekt zawieraj¹cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var sig_stop(void* value_ptr);
/** Funkcja ³aduj¹ca adres instrukcji do rejestru.
@param value_ptr wskaŸnik na obiekt zawieraj¹cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var sig_load_instruction(void* value_ptr);
/** Funkcja przepisuj¹ca wartoœæ z jednej magistrali przez drug¹ do trzeciej.
@param value_ptr wskaŸnik na obiekt zawieraj¹cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var sig_connect_buses(void* value_ptr);
/** Funkcja obs³uguj¹ca wejœcie/wyjœcie.
@param value_ptr wskaŸnik na obiekt zawieraj¹cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk³adu, 'OUTPUT_ALREADY_SET' gdy dany uk³ad posiada ju¿ wartoœæ */
var sig_io_handling(void* value_ptr);

/** Funkcja zwracaj¹ca iloczyn logiczny danej wartoœci i maski .
@param value wartoœæ
@param mask maska
@return wynik operacji */
var sig_pass(var value, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny danej wartoœci plus jeden i maski .
@param value wartoœæ
@param mask maska
@return wynik operacji */
var sig_increment(var value, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny danej wartoœci minus jeden i maski .
@param value wartoœæ
@param mask maska
@return wynik operacji */
var sig_decrement(var value, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny danej wartoœci przesuniêtej bitowo w prawo o jeden i maski .
@param value wartoœæ
@param mask maska
@return wynik operacji */
var sig_shift_right(var value, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny dope³nienia bitowego danej wartoœci i maski .
@param value wartoœæ
@param mask maska
@return wynik operacji */
var sig_bitwise_complement(var value, var mask);

/** Funkcja zwracaj¹ca iloczyn logiczny sumy dwóch wartoœci i maski .
@param value_a  pierwsza wartoœæ
@param value_b  druga wartoœæ
@param mask maska
@return wynik operacji */
var sig_add(var value_a, var value_b, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny ró¿nicy dwóch wartoœci i maski .
@param value_a  pierwsza wartoœæ
@param value_b  druga wartoœæ
@param mask maska
@return wynik operacji */
var sig_substract(var value_a, var value_b, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny iloczynu dwóch wartoœci i maski .
@param value_a  pierwsza wartoœæ
@param value_b  druga wartoœæ
@param mask maska
@return wynik operacji */
var sig_multiply(var value_a, var value_b, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny ilorazu dwóch wartoœci i maski .
@param value_a  pierwsza wartoœæ
@param value_b  druga wartoœæ
@param mask maska
@return wynik operacji */
var sig_divide(var value_a, var value_b, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny sumy logicznej dwóch wartoœci i maski .
@param value_a  pierwsza wartoœæ
@param value_b  druga wartoœæ
@param mask maska
@return wynik operacji */
var sig_bitwise_or(var value_a, var value_b, var mask);
/** Funkcja zwracaj¹ca iloczyn logiczny iloczynu logicznego dwóch wartoœci i maski .
@param value_a  pierwsza wartoœæ
@param value_b  druga wartoœæ
@param mask maska
@return wynik operacji */
var sig_bitwise_and(var value_a, var value_b, var mask);

#endif