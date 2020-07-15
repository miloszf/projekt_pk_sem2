#ifndef CPU_FUNCTIONS_H
#define CPU_FUNCTIONS_H

#include <stdbool.h>
#include "settings.h"

/** Struktura przechowuj�ca informacje potrzebne funkcji obs�uguj�cej pami�� maszyny. */
struct SignalMemory
{
	var** memory;		///< wska�nik na pami��
	struct Unit* reg_a;	///< rejestr zawieraj�cy adres
	struct Unit* reg_s;	///< rejestr zawieraj�cy s�owo
};
/** Struktura przechowuj�ca informacje potrzebne funkcji obs�uguj�cej przerwania. */
struct SignalInterrupts
{
	struct Unit* reg_rz;			///< rejestr zg�osze�
	struct Unit* reg_rm;			///< rejestr maski
	struct Unit* reg_rp;			///< rejestr przerwa�
	struct Unit* reg_ap;			///< rejestr adres�w przerwa�
	bool* int_tag;					///< wska�nik na warto�� tagu przerwa�
	const var* intr_mask;			///< wska�nik na mask� przerwa�
	struct Drawable** buttons_array;///< wska�nik na tablic� prazycisk�w przerwa�
};
/** Struktura przechowuj�ca informacje potrzebne funkcjom zwi�zanych z obs�ug� instrukcji. */
struct SignalInstruction
{
	struct Unit* from;		///< uk�ad 'z'
	struct Unit* to;		///< uk�ad 'do'
	const var* addr_len;	///< ilo�� bit�w adresowych
	const var* instr_num;	///< ilo�� instrukcji
};
/** Struktura przechowuj�ca informacje potrzebne funkcjom obs�uguj�cym po��czenia mi�dzymagistralowe. */
struct SignalBusConnection
{
	struct Unit* bus_a;		///< pierwsza magistrala
	struct Unit* bus_b;		///< druga magistrala
	struct Unit* through;	///< po�rednia magistrala
	const var* mask;		///< wska�nik na mask�
};
/** Struktura przechowuj�ca informacje potrzebne funkcjom obs�uguj�cym operacje wej�cia/wyj�cia. */
struct SignalIOHandling
{
	bool* input_flag;			///< wska�nik na flag� gotowo�ci do pobrania znaku 
	bool* output_flag;			///< wska�nik na flag� gotowo�ci do zapisu znaku 
	struct Unit* address_reg;	///< rejestr instrukcji
	const var* addr_mask;		///< wska�nik na mask� adresow�
};

/** Funkcja odczytuj�ca dane z pami�ci.
@param value_ptr wska�nik na obiekt zawieraj�cy potrzebne informacje. 
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var sig_read_from_memory(void* value_ptr);
/** Funkcja zapisuj�ca dane do pami�ci.
@ param value_ptr wska�nik na obiekt zawieraj�cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var sig_write_to_memory(void* value_ptr);
/** Funkcja odzczytuj�ca zg�oszone przerwania.
@param value_ptr wska�nik na obiekt zawieraj�cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var sig_enable_interrupts(void* value_ptr);
/** Funkcja resetuj�ca rejsetry zwi�zane z obs�u�onym przerwaniem.
@param value_ptr wska�nik na obiekt zawieraj�cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var sig_reset_interrupts(void* value_ptr);
/** Funkcja zatrzumuj�ca prac� maszyny.
@param value_ptr wska�nik na obiekt zawieraj�cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var sig_stop(void* value_ptr);
/** Funkcja �aduj�ca adres instrukcji do rejestru.
@param value_ptr wska�nik na obiekt zawieraj�cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var sig_load_instruction(void* value_ptr);
/** Funkcja przepisuj�ca warto�� z jednej magistrali przez drug� do trzeciej.
@param value_ptr wska�nik na obiekt zawieraj�cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var sig_connect_buses(void* value_ptr);
/** Funkcja obs�uguj�ca wej�cie/wyj�cie.
@param value_ptr wska�nik na obiekt zawieraj�cy potrzebne informacje.
@return '0' przy powodzeniu, 'EMPTY' dla pustego uk�adu, 'OUTPUT_ALREADY_SET' gdy dany uk�ad posiada ju� warto�� */
var sig_io_handling(void* value_ptr);

/** Funkcja zwracaj�ca iloczyn logiczny danej warto�ci i maski .
@param value warto��
@param mask maska
@return wynik operacji */
var sig_pass(var value, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny danej warto�ci plus jeden i maski .
@param value warto��
@param mask maska
@return wynik operacji */
var sig_increment(var value, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny danej warto�ci minus jeden i maski .
@param value warto��
@param mask maska
@return wynik operacji */
var sig_decrement(var value, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny danej warto�ci przesuni�tej bitowo w prawo o jeden i maski .
@param value warto��
@param mask maska
@return wynik operacji */
var sig_shift_right(var value, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny dope�nienia bitowego danej warto�ci i maski .
@param value warto��
@param mask maska
@return wynik operacji */
var sig_bitwise_complement(var value, var mask);

/** Funkcja zwracaj�ca iloczyn logiczny sumy dw�ch warto�ci i maski .
@param value_a  pierwsza warto��
@param value_b  druga warto��
@param mask maska
@return wynik operacji */
var sig_add(var value_a, var value_b, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny r�nicy dw�ch warto�ci i maski .
@param value_a  pierwsza warto��
@param value_b  druga warto��
@param mask maska
@return wynik operacji */
var sig_substract(var value_a, var value_b, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny iloczynu dw�ch warto�ci i maski .
@param value_a  pierwsza warto��
@param value_b  druga warto��
@param mask maska
@return wynik operacji */
var sig_multiply(var value_a, var value_b, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny ilorazu dw�ch warto�ci i maski .
@param value_a  pierwsza warto��
@param value_b  druga warto��
@param mask maska
@return wynik operacji */
var sig_divide(var value_a, var value_b, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny sumy logicznej dw�ch warto�ci i maski .
@param value_a  pierwsza warto��
@param value_b  druga warto��
@param mask maska
@return wynik operacji */
var sig_bitwise_or(var value_a, var value_b, var mask);
/** Funkcja zwracaj�ca iloczyn logiczny iloczynu logicznego dw�ch warto�ci i maski .
@param value_a  pierwsza warto��
@param value_b  druga warto��
@param mask maska
@return wynik operacji */
var sig_bitwise_and(var value_a, var value_b, var mask);

#endif