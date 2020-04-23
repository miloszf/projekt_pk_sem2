#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#include "error.h"
#include "vector.h"
#include "terminal.h"
#include "event.h"
//#include "window.h"
//#include "canvas.h"
#include "graphics.h"
#include "cpu.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


int main()
{
	setlocale(LC_ALL, "Polish_Poland.1250");
	struct Terminal* term = terminal_init(NULL);
	if (error())
		printf("B³¹d!");
	else
	{
		Error error = NO_ERROR;
		
		struct Window* window = window_init();
		struct Canvas* canvas = window_new_canvas(window, (Point) {20, 10 }, (Point) { 80, 20 });
		//add_rectangle(canvas, (Point) { 5, 2 }, (Point) { 10, 3 });

		struct CPU* cpu = cpu_init();

		bool exit = false;
		while (!exit && !error)
		{
			struct Vector* events_vect = event_get(term);
			struct InputEvent* event_ptr;
			while (event_ptr = vector_pop(events_vect))
			{
				if (event_ptr->type == EVENT_KEY)
				{
					if (event_ptr->key_event.key == 27)
						exit = true;
					//else if (event_ptr->key_event.key_down)
					//{
					//	for (unsigned int i = 0; i < event_ptr->key_event.repeat_count; i++)
					//		printf("%c", event_ptr->key_event.key);
					//	printf("\n");
					//}
				}
			}
			vector_delete(events_vect);
			events_vect = NULL;

			window_draw(window);
			terminal_display(term, window);
		}
		cpu_delete(cpu);
		window_delete(window);
	}
	
	terminal_del(term);
	term = NULL;
	_CrtDumpMemoryLeaks();
	return 0;
}