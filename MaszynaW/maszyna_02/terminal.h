#ifndef TERMINAL_H
#define TERMINAL_H

#include "window.h"

// "Bilbioteka" zajmuj�ca si� obs�ug� terminala

// Struktura przechowuj�ca uchwyty do wej�cia i wyj�cia konsoli
struct Terminal;

// Funkcja inicjalizuj�ca terminal. Zapisuje pocz�tkowe ustawienia konsoli,
// w��cza odpowiednie opcje, zmienia bufor na alternatywny, dodaje funkcj�
// "awaryjn�" do atexit(), ustawia globalny wska�nik na terminal(...)
// Zwraca wska�nik na now� struktur� terminal.
// argumenty: wska�nik na okno do zainicjalizowania, [nazwa okna terminala]
struct Terminal* terminal_init(const char* window_name);

// Funkcja przepisuj�ca bufor okna do konsoli
void terminal_display(struct Terminal* term, struct Window* window);

// Funkcja usuwaj�ca terminal.
void terminal_del(struct Terminal* terminal);

#endif