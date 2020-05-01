#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#include "error.h"
#include "vector.h"
#include "terminal.h"
#include "event.h"
#include "graphics.h"
#include "cpu.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main()
{
	struct Window* window = window_init();
	struct Canvas* canvas = window_new_canvas(window, (Point) {0, 0 }, (Point) { 70, 31 });
	struct CPU* cpu = cpu_init(canvas);

	cpu_delete(cpu);
	window_delete(window);
	debug_error_delete();
	_CrtDumpMemoryLeaks();
	return 0;
}

//int main()
//{
//	setlocale(LC_ALL, "Polish_Poland.1250");
//	struct Terminal* term = terminal_init(NULL);
//	if (error_get())
//		printf("B³¹d!");
//	else
//	{
//		Error error = NO_ERROR;
//		
//		struct Window* window = window_init();
//		struct Canvas* canvas = window_new_canvas(window, (Point) {0, 0 }, (Point) { 70, 31 });
//		/*struct Drawable* new_reg = drawable_new_reg(canvas, (Point) { 5, 2 }, (Point) { 10, 3 }, "AK" );
//		drawable_set_visibility(new_reg, true);
//		var reg_value = 0;
//		struct Drawable* new_bus = drawable_new_bus(canvas, (Point) { 4, 8 }, (Point) { 0, 20 });
//		drawable_set_visibility(new_bus, true);
//		struct Drawable* new_comb = drawable_new_comb(canvas, (Point) { 5, 4 }, (Point) { 20, 3 });
//		drawable_set_visibility(new_comb, true);
//
//		struct DrawableSignalInit drawable_signal_init = (struct DrawableSignalInit){
//			.canvas = canvas,
//			.arrow.head = (Point){14, 6},
//			.arrow.tail = (Point){14, 10},
//			.tag.type = TO_BUS_TYPE,
//			.tag.head = (Point){15, 7},
//			.tag.tail = (Point){24, 7},
//		};
//		struct Drawable* new_sig = drawable_new_signal(&drawable_signal_init, "weja");
//		drawable_set_visibility(new_sig, true);*/
//		//add_rectangle(canvas, (Point) { 5, 2 }, (Point) { 10, 3 });
//
//		struct CPU* cpu = cpu_init(canvas);
//
//		bool exit = false;
//		while (!exit && !error)
//		{
//			struct Vector* events_vect = event_get(term);
//			struct InputEvent* event_ptr;
//			while (event_ptr = vector_pop(events_vect))
//			{
//				if (event_ptr->type == EVENT_KEY)
//				{
//					if (event_ptr->key_event.key == 27)
//						exit = true;
//					/*else if (event_ptr->key_event.key == 'r' && event_ptr->key_event.key_down == true)
//					{
//						reg_value++;
//						drawable_set_value(new_reg, &reg_value);
//					}
//					else if (event_ptr->key_event.key == 'b')
//					{
//						var bus_value = event_ptr->key_event.key_down ? 0 : EMPTY;
//						drawable_set_value(new_bus, &bus_value);
//					}
//					else if (event_ptr->key_event.key == 's')
//					{
//						var sig_value = event_ptr->key_event.key_down ? 0 : EMPTY;
//						drawable_set_value(new_sig, &sig_value);
//					}*/
//
//					//else if (event_ptr->key_event.key_down)
//					//{
//					//	for (unsigned int i = 0; i < event_ptr->key_event.repeat_count; i++)
//					//		printf("%c", event_ptr->key_event.key);
//					//	printf("\n");
//					//}
//				}
//			}
//			vector_delete(events_vect);
//			events_vect = NULL;
//
//			window_draw(window);
//			terminal_display(term, window);
//		}
//		cpu_delete(cpu);
//		window_delete(window);
//	}
//	
//	terminal_del(term);
//	term = NULL;
//	_CrtDumpMemoryLeaks();
//	return 0;
//}