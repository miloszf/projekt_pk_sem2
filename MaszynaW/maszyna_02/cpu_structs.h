#ifndef CPU_STRUCTS_H
#define CPU_STRUCTS_H

#include <stdbool.h>
#include "cpu.h"
#include "settings.h"

/** Struktura zawieraj¹ca informacje dotycz¹ce pojedyñczego ustawienia maszyny. */
struct CPUPreference
{
	const char* name;			///< nazwa ustawienia
	unsigned char value;		///< wartoœæ ustawienia (zale¿na od typu)
	struct Vector* unit_vect;	///< wektor uk³adów powi¹zanych z dan¹ opcj¹
	struct Vector* signal_vect;	///< wektor sygna³ów powi¹zanych z dan¹ opcj¹
};

#define CPU_SETUP_SIZE 13
#define CPU_TAGS_NUMBER 4
#define CPU_INTERRUPTS_NUMBER 4

/** struktura przechowuj¹ca wszystkie opcje maszyny. */
struct CPUSetup
{
	union
	{
		struct {
			struct CPUPreference addr_length;	///< iloœæ bitów adresowych
			struct CPUPreference code_length;	///< iloœæ bitów kodu
			struct CPUPreference bus_link;		///< po³¹czenie miedzymagistralowe
			struct CPUPreference alu_inc;		///< inkrementacja i dekrementacja akumulatora
			struct CPUPreference alu_logic;		///< operacje logiczne w JAL
			struct CPUPreference alu_ext;		///< rozszerzone operacje w JAL
			struct CPUPreference stack;			///< obs³uga stosu
			struct CPUPreference reg_x;			///< rejestr X
			struct CPUPreference reg_y;			///< rejestr Y
			struct CPUPreference interrupts;	///< obs³uga przerwañ
			struct CPUPreference io;			///< obs³uga wejœcia/wyjœcia
			struct CPUPreference tags;			///< znaczniki (nieobs³ugiwane)
			struct CPUPreference basic;			///< podstawowy zestaw uk³adów i sygna³ów (zawsze '1')
		} all;
		struct CPUPreference list[CPU_SETUP_SIZE];	///< lista zawieraj¹ca wszystkie ustawienia maszyny
	};
};

/** Typ wyliczeniowy reprezentuj¹cy rodzaj tagu. */
typedef enum { TAG_Z, TAG_ZAK, TAG_V, TAG_INT } CPUTagType;
/** Struktura reprezentuj¹ca tag - zmienn¹ logiczn¹ uzale¿nion¹ od stanu
	danych uk³adów maszyny. */
struct CPUTag
{
	const char* name;	///< nazwa tagu
	bool value;			///< wartoœæ logiczna tagu
	CPUTagType type;	///< typ tagu
};	
/** Struktura zawieraj¹ca wszystkie uk³ady, sygna³y i tagi maszyny. */
struct CPUComponents
{
	/** Jednostka arytmetyczno-logiczna. */
	struct
	{
		struct Unit* reg_ak;
		struct Unit* comb_weja;
		struct Unit* comb_weak;
		struct Signal* sig_weja;
		struct Signal* sig_shr;
		struct Signal* sig_dziel;
		struct Signal* sig_mno;
		struct Signal* sig_przep;
		struct Signal* sig_ode;
		struct Signal* sig_dod;
		struct Signal* sig_weak;
		struct Signal* sig_iak;
		struct Signal* sig_wyak;
		struct Signal* sig_dak;
		struct Signal* sig_neg;
		struct Signal* sig_lub;
		struct Signal* sig_i;
	} alu;
	/** Pamiêæ. */
	struct {
		struct Unit* reg_a;
		struct Unit* reg_s;
		struct Signal* sig_czyt;
		struct Signal* sig_pisz;
		struct Signal* sig_wea;
		struct Signal* sig_wys;
		struct Signal* sig_wes;
	} mem;
	/** Magistrale, sygna³y i uk³ady zwi¹zane z przetwarzaniem adresu. */
	struct {
		struct Unit* reg_i;
		struct Unit* reg_l;
		struct Unit* bus_s;
		struct Unit* bus_a;
		struct Unit* bus_as;
		struct Signal* sig_wyl;
		struct Signal* sig_wel;
		struct Signal* sig_wyls;
		struct Signal* sig_il;
		struct Signal* sig_wyad;
		struct Signal* sig_wei;
		struct Signal* sig_as;
		struct Signal* sig_sa;
		struct Signal* sig_stop;
	} addr;
	/** Rejestry X i Y. */
	struct {
		struct Unit* reg_x;
		struct Unit* reg_y;
		struct Signal* sig_wyx;
		struct Signal* sig_wex;
		struct Signal* sig_wyy;
		struct Signal* sig_wey;
	} xy;
	/** Obs³uga stosu. */
	struct {
		struct Unit* reg_ws;
		struct Signal* sig_wyws;
		struct Signal* sig_wews;
		struct Signal* sig_iws;
		struct Signal* sig_dws;
	} stack;
	/** Obs³uga wejœcia/wyjœcia. */
	struct {
		struct Unit* reg_rb;
		struct Unit* reg_g;
		struct Signal* sig_wyrb;
		struct Signal* sig_werb;
		struct Signal* sig_wyg;
		struct Signal* sig_start;
	} io;
	/** Obs³uga przerwañ. */
	struct {
		struct Unit* reg_rz;
		struct Unit* reg_rm;
		struct Unit* reg_rp;
		struct Unit* reg_ap;
		struct Signal* sig_wyrm;
		struct Signal* sig_werm;
		struct Signal* sig_rint;
		struct Signal* sig_eni;
		struct Signal* sig_wyap;
	} intr;

	union
	{
		struct
		{
			struct CPUTag tag_int;
			struct CPUTag tag_v;
			struct CPUTag tag_z;
			struct CPUTag tag_zak;
		} all;
		struct CPUTag list[CPU_TAGS_NUMBER];
	}tags;

};

/** Struktura reprezentuj¹ca pamiêæ maszyny. */
struct CPUMemory
{
	var* memory_array;					///< pamiêæ maszyny, tablica wartoœci
	const var* addr_len;				///< wskaŸnik na iloœæ bitów adresowych
	struct Drawable* drawable;			///< reprezentacja graficzna pamiêci
	struct Vector* instr_names_vect;	///< wektor instrukcji
};
/** Struktura przechowuj¹ca informacje zwi¹zane z iloœci¹ bitów oraz maskami maszyny. */
struct CPUWord
{
	var word_len;	///< iloœæ bitów s³owa maszyny
	var addr_len;	///< iloœæ bitów adresowych
	var word_mask;	///< maska s³owa
	var addr_mask;	///< maska adresu
	var char_mask;	///< mask znaku uk³ady io
	var intr_mask;	///< maska przerwañ
	var instr_num;	///< liczba zapisanych instrukcji
};
/** Struktura przechowuj¹ca informacje wykorzystywane podczas wykonywania programu. */
struct CPURuntime
{
	struct Instruction* current_instr;	///< obecnie wykonywana instrukcja
	struct Tick* current_tick;			///< obecnie wykonywany takt
	bool stop;							///< czy maszyna wstrzyma³a pracê
};
/** Struktura przechowuj¹ca informacje o uk³adach peryferyjnych maszyny. */
struct CPUPeripherals
{
	bool input_flag;										///< flaga gotowoœci do pobrania znaku
	bool output_flag;										///< flaga gotowoœci do wys³ania znaku
	struct Drawable* buttons_array[CPU_INTERRUPTS_NUMBER];	///< tablica reprezentacji graficznych przycisków przerwañ
	var buttons_set;										///< stan przycisków przerwañ
};
/** Struktura reprezentuj¹ca jednostkê centraln¹ maszyny. */
struct CPU
{
	struct CPUSetup setup;				///< obiekt przechowuj¹cy ustawienia maszyny
	struct CPUComponents components;	///< obiekt zawieraj¹cy wszystkie uk³ady, sygna³y i tagi maszyny
	struct CPUWord word;				///< obiekt przechowuj¹cy informacje zwi¹zane z iloœci¹ bitów oraz maskami maszyny
	struct CPUMemory* memory;			///< obiekt przechowuj¹cy informacje dotycz¹ce pamiêci maszyny
	/** Struktura przechowuj¹ca wektory uk³adów, sygna³ów i instrukcji zgodnych z bie¿¹cymi ustawieniami. */
	struct
	{
		struct Vector* units;			///< wektor uk³adów
		struct Vector* signals;			///< wektor sygna³ów
		struct Vector* instructions;	///< wektor instrukcji
	} vector;
	struct CPURuntime runtime;			///< obiekt przechowuj¹cy informacje wykorzystywane podczas wykonywania programu
	struct CPUPeripherals peripherals;	///< obiekt przechowuj¹cy informacje o uk³adach peryferyjnych maszyny
};

/** Funkcja inicjalizuj¹ca obiekt struktury CPUTag.
@param name nazwa tagu
@param type rodzaj tagu
@return nowy obiekt */
struct CPUTag cpu_tag_init(const char* name, CPUTagType type);
/** Funkcja odœwie¿aj¹ca dany tag.
@param tag tag
@param cpu jednostka centralna
@return wartoœæ logiczna tagu */
bool cpu_tag_update(struct CPUTag* tag, struct CPU* cpu);
/** Funkcja usuwaj¹ca tag.
@param tag tag do usuniêcia */
void cpu_tag_delete(struct CPUTag* tag);

/** Funkcja inicjalizuj¹ca pamiêæ maszyny.
@param canvas scena w której zawiera siê pamiêæ
@param position pozycja wzglêdem lewego górnego rogu sceny
@param addr_length wskaŸnik na iloœæ bitów adresowych
@return nowy obiekt */
struct CPUMemory* cpu_memory_init(struct Canvas* canvas, const Point position, const var* addr_length);
/** Funkcja prze³adowuj¹ca program do pamiêci maszysny.
@param memory pamiêæ
@param instr_vect wektor instrukcji */
void cpu_memory_update(struct CPUMemory* memory, struct Vector* instr_vect);
/** Funkcja przewijaj¹ca graficzn¹ reprezentacjê pamiêci.
@param memory pamiêæ
@param offset przesuniêcie */
void cpu_memory_scroll(struct CPUMemory* memory, var offset);
/** Funkcja usuwaj¹ca podany obiekt
@param window obiekt do usuniêcia */
void cpu_memory_delete(struct CPUMemory*);

/** Funkcja zwracaj¹ca zainicjalizowany obiekt struktury CPUWord.
@return nowy obiekt */
struct CPUWord cpu_word_init();
/** Funkcja odœwie¿aj¹ca informacje dotycz¹ce iloœci bitów i masek maszyny.
@param word wskaŸnik na obiekt do odœwie¿enia
@param code_length iloœæ bitów kodu
@param address_length iloœæ bitów adresowych
@param instr_vect wektor instrukcji */
void  cpu_word_update(struct CPUWord* word, var code_length, var address_length, struct Vector* instr_vect);
/** Funkcja odœwie¿aj¹ca stan przycisków przerwañ maszyny.
@param interrupts zg³oszone przerwania
@param buttons_array tablica reprezentacji graficznych przycisków */
void cpu_peripherals_update_buttons(var interrupts, struct Drawable** buttons_array);

#endif