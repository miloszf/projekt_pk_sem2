#ifndef CPU_STRUCTS_H
#define CPU_STRUCTS_H

#include <stdbool.h>
#include "cpu.h"
#include "settings.h"

/** Struktura zawieraj�ca informacje dotycz�ce pojedy�czego ustawienia maszyny. */
struct CPUPreference
{
	const char* name;			///< nazwa ustawienia
	unsigned char value;		///< warto�� ustawienia (zale�na od typu)
	struct Vector* unit_vect;	///< wektor uk�ad�w powi�zanych z dan� opcj�
	struct Vector* signal_vect;	///< wektor sygna��w powi�zanych z dan� opcj�
};

#define CPU_SETUP_SIZE 13
#define CPU_TAGS_NUMBER 4
#define CPU_INTERRUPTS_NUMBER 4

/** struktura przechowuj�ca wszystkie opcje maszyny. */
struct CPUSetup
{
	union
	{
		struct {
			struct CPUPreference addr_length;	///< ilo�� bit�w adresowych
			struct CPUPreference code_length;	///< ilo�� bit�w kodu
			struct CPUPreference bus_link;		///< po��czenie miedzymagistralowe
			struct CPUPreference alu_inc;		///< inkrementacja i dekrementacja akumulatora
			struct CPUPreference alu_logic;		///< operacje logiczne w JAL
			struct CPUPreference alu_ext;		///< rozszerzone operacje w JAL
			struct CPUPreference stack;			///< obs�uga stosu
			struct CPUPreference reg_x;			///< rejestr X
			struct CPUPreference reg_y;			///< rejestr Y
			struct CPUPreference interrupts;	///< obs�uga przerwa�
			struct CPUPreference io;			///< obs�uga wej�cia/wyj�cia
			struct CPUPreference tags;			///< znaczniki (nieobs�ugiwane)
			struct CPUPreference basic;			///< podstawowy zestaw uk�ad�w i sygna��w (zawsze '1')
		} all;
		struct CPUPreference list[CPU_SETUP_SIZE];	///< lista zawieraj�ca wszystkie ustawienia maszyny
	};
};

/** Typ wyliczeniowy reprezentuj�cy rodzaj tagu. */
typedef enum { TAG_Z, TAG_ZAK, TAG_V, TAG_INT } CPUTagType;
/** Struktura reprezentuj�ca tag - zmienn� logiczn� uzale�nion� od stanu
	danych uk�ad�w maszyny. */
struct CPUTag
{
	const char* name;	///< nazwa tagu
	bool value;			///< warto�� logiczna tagu
	CPUTagType type;	///< typ tagu
};	
/** Struktura zawieraj�ca wszystkie uk�ady, sygna�y i tagi maszyny. */
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
	/** Pami��. */
	struct {
		struct Unit* reg_a;
		struct Unit* reg_s;
		struct Signal* sig_czyt;
		struct Signal* sig_pisz;
		struct Signal* sig_wea;
		struct Signal* sig_wys;
		struct Signal* sig_wes;
	} mem;
	/** Magistrale, sygna�y i uk�ady zwi�zane z przetwarzaniem adresu. */
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
	/** Obs�uga stosu. */
	struct {
		struct Unit* reg_ws;
		struct Signal* sig_wyws;
		struct Signal* sig_wews;
		struct Signal* sig_iws;
		struct Signal* sig_dws;
	} stack;
	/** Obs�uga wej�cia/wyj�cia. */
	struct {
		struct Unit* reg_rb;
		struct Unit* reg_g;
		struct Signal* sig_wyrb;
		struct Signal* sig_werb;
		struct Signal* sig_wyg;
		struct Signal* sig_start;
	} io;
	/** Obs�uga przerwa�. */
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

/** Struktura reprezentuj�ca pami�� maszyny. */
struct CPUMemory
{
	var* memory_array;					///< pami�� maszyny, tablica warto�ci
	const var* addr_len;				///< wska�nik na ilo�� bit�w adresowych
	struct Drawable* drawable;			///< reprezentacja graficzna pami�ci
	struct Vector* instr_names_vect;	///< wektor instrukcji
};
/** Struktura przechowuj�ca informacje zwi�zane z ilo�ci� bit�w oraz maskami maszyny. */
struct CPUWord
{
	var word_len;	///< ilo�� bit�w s�owa maszyny
	var addr_len;	///< ilo�� bit�w adresowych
	var word_mask;	///< maska s�owa
	var addr_mask;	///< maska adresu
	var char_mask;	///< mask znaku uk�ady io
	var intr_mask;	///< maska przerwa�
	var instr_num;	///< liczba zapisanych instrukcji
};
/** Struktura przechowuj�ca informacje wykorzystywane podczas wykonywania programu. */
struct CPURuntime
{
	struct Instruction* current_instr;	///< obecnie wykonywana instrukcja
	struct Tick* current_tick;			///< obecnie wykonywany takt
	bool stop;							///< czy maszyna wstrzyma�a prac�
};
/** Struktura przechowuj�ca informacje o uk�adach peryferyjnych maszyny. */
struct CPUPeripherals
{
	bool input_flag;										///< flaga gotowo�ci do pobrania znaku
	bool output_flag;										///< flaga gotowo�ci do wys�ania znaku
	struct Drawable* buttons_array[CPU_INTERRUPTS_NUMBER];	///< tablica reprezentacji graficznych przycisk�w przerwa�
	var buttons_set;										///< stan przycisk�w przerwa�
};
/** Struktura reprezentuj�ca jednostk� centraln� maszyny. */
struct CPU
{
	struct CPUSetup setup;				///< obiekt przechowuj�cy ustawienia maszyny
	struct CPUComponents components;	///< obiekt zawieraj�cy wszystkie uk�ady, sygna�y i tagi maszyny
	struct CPUWord word;				///< obiekt przechowuj�cy informacje zwi�zane z ilo�ci� bit�w oraz maskami maszyny
	struct CPUMemory* memory;			///< obiekt przechowuj�cy informacje dotycz�ce pami�ci maszyny
	/** Struktura przechowuj�ca wektory uk�ad�w, sygna��w i instrukcji zgodnych z bie��cymi ustawieniami. */
	struct
	{
		struct Vector* units;			///< wektor uk�ad�w
		struct Vector* signals;			///< wektor sygna��w
		struct Vector* instructions;	///< wektor instrukcji
	} vector;
	struct CPURuntime runtime;			///< obiekt przechowuj�cy informacje wykorzystywane podczas wykonywania programu
	struct CPUPeripherals peripherals;	///< obiekt przechowuj�cy informacje o uk�adach peryferyjnych maszyny
};

/** Funkcja inicjalizuj�ca obiekt struktury CPUTag.
@param name nazwa tagu
@param type rodzaj tagu
@return nowy obiekt */
struct CPUTag cpu_tag_init(const char* name, CPUTagType type);
/** Funkcja od�wie�aj�ca dany tag.
@param tag tag
@param cpu jednostka centralna
@return warto�� logiczna tagu */
bool cpu_tag_update(struct CPUTag* tag, struct CPU* cpu);
/** Funkcja usuwaj�ca tag.
@param tag tag do usuni�cia */
void cpu_tag_delete(struct CPUTag* tag);

/** Funkcja inicjalizuj�ca pami�� maszyny.
@param canvas scena w kt�rej zawiera si� pami��
@param position pozycja wzgl�dem lewego g�rnego rogu sceny
@param addr_length wska�nik na ilo�� bit�w adresowych
@return nowy obiekt */
struct CPUMemory* cpu_memory_init(struct Canvas* canvas, const Point position, const var* addr_length);
/** Funkcja prze�adowuj�ca program do pami�ci maszysny.
@param memory pami��
@param instr_vect wektor instrukcji */
void cpu_memory_update(struct CPUMemory* memory, struct Vector* instr_vect);
/** Funkcja przewijaj�ca graficzn� reprezentacj� pami�ci.
@param memory pami��
@param offset przesuni�cie */
void cpu_memory_scroll(struct CPUMemory* memory, var offset);
/** Funkcja usuwaj�ca podany obiekt
@param window obiekt do usuni�cia */
void cpu_memory_delete(struct CPUMemory*);

/** Funkcja zwracaj�ca zainicjalizowany obiekt struktury CPUWord.
@return nowy obiekt */
struct CPUWord cpu_word_init();
/** Funkcja od�wie�aj�ca informacje dotycz�ce ilo�ci bit�w i masek maszyny.
@param word wska�nik na obiekt do od�wie�enia
@param code_length ilo�� bit�w kodu
@param address_length ilo�� bit�w adresowych
@param instr_vect wektor instrukcji */
void  cpu_word_update(struct CPUWord* word, var code_length, var address_length, struct Vector* instr_vect);
/** Funkcja od�wie�aj�ca stan przycisk�w przerwa� maszyny.
@param interrupts zg�oszone przerwania
@param buttons_array tablica reprezentacji graficznych przycisk�w */
void cpu_peripherals_update_buttons(var interrupts, struct Drawable** buttons_array);

#endif