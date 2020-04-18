// Struktura bêd¹ca "sercem" maszynyW. Inicjalizuje bloki, sygna³y, wczytuje rozkazy i program.
// Wykonuje takty lub rozkazy. Poœredniczy miêdzy sygna³ami zewnêtrznymi (przerwania, we/wy klawiatury)

struct CPU;

// Funkcja tworz¹ca now¹ jednostkê centraln¹
// Nowa jednostka jest ju¿ wstêpnie zainicjalizowana, tj. utworzone s¹ bloki, sygna³y,
// odpowiednie struktury i po³¹czenia
// argumenty: [?], bufor znaków z klawiatury do odczytania, bufor znaków do zapisania
// zwraca: 

// Funkcja wczytuj¹ca i kompiluj¹ca listê rozkazów i ustawieñ
// argumenty: nazwa pliku z rozkazami

// Funkcja wczytuj¹ca i kompiluj¹ca program. 
// argumenty: nazwa pliku z programem

// Funkcja wykonuj¹ca jeden takt, zwraca wartoœæ 
// argumenty: [?]

// Funkcja wykonuj¹ca jeden rozkaz
// NIE MA!!! (mo¿liwa blokada)

// Funkcja zapisuj¹ca zg³oszone przerwanie do maszyny
// argumenty: przerwania do zapisania

// Funkcja resetuj¹ca maszynê (bez pamiêci)
// argumenty: -

// Funkcja rysuj¹ca aktualny stan maszyny