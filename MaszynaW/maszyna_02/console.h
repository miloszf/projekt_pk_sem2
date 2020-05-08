#ifndef CONSOLE
#define CONSOLE

struct Console;

struct Console* console_init(struct Canvas* canvas);

void console_delete(struct Console* console);

#endif