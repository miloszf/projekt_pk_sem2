#include "console.h"
#include "graphics.h"
#include "error.h"
#include "settings.h"

struct Console
{
	wchar** buffer;
	Point buffer_size;
};

struct Console* console_init(struct Canvas* canvas, Point size)
{
	CHECK_IF_NULL(canvas);
	if ((size.x < 3) || (!size.y < 3))
		CRASH_LOG(LOG_UNKNOWN_VALUE);

	struct Console* new_console = malloc_s(sizeof(struct Console));
	new_console->buffer_size = POINT(size.x - 2, size.y - 2);
	new_console->buffer = malloc_s(sizeof(wchar*) * new_console->buffer_size.y);
	for (int i = 0; i < new_console->buffer_size.y; i++)
		new_console->buffer[i] = calloc_s(new_console->buffer_size.x, sizeof(wchar));
	drawable_new_frame(canvas, POINT(0, 0), size);
	
}

void console_delete(struct Console* console)
{

}