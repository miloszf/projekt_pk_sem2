#ifndef CONSOLE_H
#define CONSOLE_H

#include "settings.h"

struct Console;

struct Console* console_init(struct Canvas* canvas, Point position, Point size);
// DEBUG
//void scroll(struct Console* console);

void console_print(struct Console* console, const char* text);
char console_get_char(struct Console* console);
void console_clear(struct Console* console);
void console_delete(struct Console* console);

#endif