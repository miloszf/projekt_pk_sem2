// "Bilbioteka" zajmuj�ca si� obs�ug� terminala

// Struktura przechowuj�ca uchwyty do wej�cia i wyj�cia konsoli
// Singleton?

// Funkcja inicjalizuj�ca terminal. Zapisuje pocz�tkowe ustawienia konsoli,
// w��cza odpowiednie opcje, zmienia bufor na alternatywny, dodaje funkcj�
// "awaryjn�" do atexit(), ustawia globalny wska�nik na terminal(...)
// Zwraca wska�nik na now� struktur� terminal.
// argumenty: [nazwa okna]

// Funkcja usuwaj�ca terminal. Przywraca pocz�tkowe ustawienia konsoli.

// Funkcja "awaryjna", nieczego nie zwraca. Wykonywana na sam koniec programu,
// wywo�uje funkcj� usuwaj�c� terminal, nast�pnie wypisuje b��d na standardowe wyj�cie. 
// argumenty: brak [atexit() przyjmuje void(*fun)(void)]

// Funkcja zwracaj�ca uchwyt do wyj�cia konsoli
// argumenty: wska�nik na terminal

// Funkcja zwracaj�ca uchwyt do wej�cia konsoli
// argumenty: wska�nik na terminal