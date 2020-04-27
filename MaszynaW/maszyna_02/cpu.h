// Struktura b�d�ca "sercem" maszynyW. Inicjalizuje bloki, sygna�y, wczytuje rozkazy i program.
// Wykonuje takty lub rozkazy. Po�redniczy mi�dzy sygna�ami zewn�trznymi (przerwania, we/wy klawiatury)

struct CPU;

// Funkcja tworz�ca now� jednostk� centraln�
// Nowa jednostka jest ju� wst�pnie zainicjalizowana, tj. utworzone s� bloki, sygna�y,
// odpowiednie struktury i po��czenia
// argumenty: [?], bufor znak�w z klawiatury do odczytania, bufor znak�w do zapisania
// zwraca: wska�nik na now� jednostk�
struct CPU* cpu_init(struct Canvas* canvas);

// Funkcja wczytuj�ca i kompiluj�ca list� rozkaz�w i ustawie�
// argumenty: nazwa pliku z rozkazami

// Funkcja wczytuj�ca i kompiluj�ca program. 
// argumenty: nazwa pliku z programem

// Funkcja wykonuj�ca jeden takt, zwraca warto�� 
// argumenty: [?]

// Funkcja wykonuj�ca jeden rozkaz
// NIE MA!!! (mo�liwa blokada)

// Funkcja zapisuj�ca zg�oszone przerwanie do maszyny
// argumenty: przerwania do zapisania

// Funkcja resetuj�ca maszyn� (bez pami�ci)
// argumenty: -

// Funkcja rysuj�ca aktualny stan maszyny


void cpu_delete(struct CPU* cpu);