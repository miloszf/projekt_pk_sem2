#ifndef TERMINAL_H
#define TERMINAL_H

#include "window.h"

// "Bilbioteka" zajmuj¹ca siê obs³ug¹ terminala

// Struktura przechowuj¹ca uchwyty do wejœcia i wyjœcia konsoli
struct Terminal;

// Funkcja inicjalizuj¹ca terminal. Zapisuje pocz¹tkowe ustawienia konsoli,
// w³¹cza odpowiednie opcje, zmienia bufor na alternatywny, dodaje funkcjê
// "awaryjn¹" do atexit(), ustawia globalny wskaŸnik na terminal(...)
// Zwraca wskaŸnik na now¹ strukturê terminal.
// argumenty: wskaŸnik na okno do zainicjalizowania, [nazwa okna terminala]
struct Terminal* terminal_init(const wchar* window_name);

// Funkcja przepisuj¹ca bufor okna do konsoli
void terminal_display(struct Terminal* term, struct Window* window);

// Funkcja usuwaj¹ca terminal.
void terminal_del(struct Terminal* terminal);

#endif