// "Bilbioteka" zajmuj¹ca siê obs³ug¹ terminala

// Struktura przechowuj¹ca uchwyty do wejœcia i wyjœcia konsoli
// Singleton?

// Funkcja inicjalizuj¹ca terminal. Zapisuje pocz¹tkowe ustawienia konsoli,
// w³¹cza odpowiednie opcje, zmienia bufor na alternatywny, dodaje funkcjê
// "awaryjn¹" do atexit(), ustawia globalny wskaŸnik na terminal(...)
// Zwraca wskaŸnik na now¹ strukturê terminal.
// argumenty: [nazwa okna]

// Funkcja usuwaj¹ca terminal. Przywraca pocz¹tkowe ustawienia konsoli.

// Funkcja "awaryjna", nieczego nie zwraca. Wykonywana na sam koniec programu,
// wywo³uje funkcjê usuwaj¹c¹ terminal, nastêpnie wypisuje b³¹d na standardowe wyjœcie. 
// argumenty: brak [atexit() przyjmuje void(*fun)(void)]

// Funkcja zwracaj¹ca uchwyt do wyjœcia konsoli
// argumenty: wskaŸnik na terminal

// Funkcja zwracaj¹ca uchwyt do wejœcia konsoli
// argumenty: wskaŸnik na terminal