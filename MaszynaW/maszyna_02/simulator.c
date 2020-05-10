#include <stdlib.h>
#include <stdbool.h>

#include "simulator.h"
#include "cpu.h"
#include "terminal.h"
#include "window.h"
#include "graphics.h"
#include "event.h"
#include "error.h"
#include "settings.h"

struct Simulator
{
	struct Terminal* terminal;
	struct Window* window;
	struct CPU* cpu;
	struct Console* console;
};

//struct Simulator* simulator_init(int argc, char** argv)
//{
//
//	//sim->console = console_init(cpu_canvas);
//
//	return sim;
//}

//void simulator_delete(struct Simulator* sim)
//{
//	if (sim)
//	{
//
//	}
//}

typedef enum { READY, INIT, EXIT, CLEAR, TICK, INSTR, PROGRAM, RELOAD_INSTR, RELOAD_PROGRAM} SimState;

struct UserInput
{
	SimState new_state;
	char mouse_scroll;
};

struct UserInput simulator_get_events(struct Vector* events_vect)
{
	CHECK_IF_NULL(events_vect);
	struct UserInput new_input = { READY, 0 };
	struct InputEvent* event_ptr;
	while (event_ptr = vector_pop(events_vect))
	{
		if (event_ptr->type == EVENT_KEY && event_ptr->key_event.key_down)
		{
			switch (event_ptr->key_event.key)
			{
			case 27: // ESC
				new_input.new_state = EXIT;
				break;
			case F2_KEY:
				new_input.new_state = CLEAR;
				break;
			case F7_KEY:
				new_input.new_state = TICK;
				break;
			case F8_KEY:
				new_input.new_state = INSTR;
				break;
			case F9_KEY:
				new_input.new_state = PROGRAM;
				break;
			case F11_KEY:
				new_input.new_state = RELOAD_INSTR;
				break;
			case F12_KEY:
				new_input.new_state = RELOAD_PROGRAM;
				break;
			default:
				break;
			}

			if (new_input.new_state == EXIT)
				break;
		}
		else if (event_ptr->type == EVENT_MOUSE)
		{ 
			new_input.mouse_scroll = -event_ptr->mouse_event.scroll;
		}
	}
	vector_delete(events_vect);
	//events_vect = NULL;
	return new_input;
}

void simulator_run(int argc, char** argv)
{
	init_crash_log();
	//struct Simulator* sim = calloc_s(1, sizeof(struct Simulator));
	struct Simulator sim = { NULL, NULL, NULL, NULL };
	//*sim = (struct Simulator)

	sim.terminal = terminal_init("MaszynaW");
	sim.window = window_init();
	struct Canvas* cpu_canvas = window_new_canvas(sim.window, (Point) { 0, 0 }, (Point) { 73, 31 });
	//struct Canvas* console_canvas = window_new_canvas(sim.window, (Point) { 0, 31 }, (Point) { 73, 40 });
	sim.cpu = cpu_init(cpu_canvas);
	//sim->console = console_init(cpu_canvas);

	SimState state = INIT;
	//bool exit = false;

	// DEBUG
	const char instr_file_name[] = "MaszynaW.lst";
	const char prog_file_name[] = "program.prg";

	while (state != EXIT)
	{
		bool redraw = false;
		bool log_error = false;
		struct UserInput user_input = simulator_get_events(event_get(sim.terminal));
		if (user_input.new_state == EXIT)
			state = EXIT;
		
		switch (state)
		{
		case INIT:
		{
			if (!cpu_import_instructions(sim.cpu, instr_file_name) || !cpu_import_program(sim.cpu, prog_file_name))
			{
				log_error = true;
			}
			redraw = true;
			state = READY;
		}
		break;
		case READY:
		{
			Error err = error();
			UserErrorType err_type = err & USER_ERROR_TYPE;
			err &= ~USER_ERROR_TYPE;
			const UserErrorType COMP_ERROR = INSTR_COMP_ERROR | PROG_COMP_ERROR;

			if (user_input.new_state == RELOAD_INSTR)
				state = RELOAD_INSTR;
			else if (user_input.new_state == RELOAD_PROGRAM && !(err_type & INSTR_COMP_ERROR))
				state = RELOAD_PROGRAM;
			else if (user_input.new_state == CLEAR && !(err_type & COMP_ERROR))
				state = CLEAR;
			else if (!err_type)
			{
				switch (user_input.new_state)
				{
				case TICK:
				case INSTR:
				case PROGRAM:
					state = user_input.new_state;
					break;
				case READY:
					break;
				default:
					CRASH_LOG(LOG_UNKNOWN_VALUE);
				}
			}
			else
			{
				log_error = true;
				redraw = true;
			}
		}
		break;
		case EXIT:
			break;
		case RELOAD_INSTR:
		{
			if (!cpu_import_instructions(sim.cpu, instr_file_name) || !cpu_import_program(sim.cpu, prog_file_name))
			{
				log_error = true;
			}
			redraw = true;
			state = READY;
		}
		break;
		case RELOAD_PROGRAM:
		{
			if (!cpu_import_program(sim.cpu, prog_file_name))
			{
				log_error = true;
			}
			redraw = true;
			state = READY;
		}
		break;
		case CLEAR:
		{
			//cpu_clear(sim.cpu);
			redraw = true;
			state = READY;
		}
		break;
		case TICK:
		{
			if (!cpu_tick(sim.cpu) && error())
			{
				log_error = true;
			}
			redraw = true;
			state = READY;
		}
		break;
		case INSTR:
		{
			if (user_input.new_state != READY)
				state = user_input.new_state;
			else
			{
				if (!cpu_tick(sim.cpu))
				{
					if (error())
					{
						log_error = true;
					}
					state = READY;
				}
				redraw = true;
			}
		}
		break;
		case PROGRAM:
		{
			if (user_input.new_state != READY)
				state = user_input.new_state;
			else
			{
				if (!cpu_tick(sim.cpu))
				{
					if (error())
					{
						log_error = true;
						state = READY;
					}
				}
				redraw = true;
			}
		}
		break;
		default:
			CRASH_LOG(LOG_UNKNOWN_VALUE);
		}

		if (log_error)
		{
			//console log error
		}

		if (user_input.mouse_scroll)
		{
			//memory scroll
		}
		redraw = true;
		if (redraw)
		{
			window_draw(sim.window);
			terminal_display(sim.terminal, sim.window);
		}
	}

	//console_delete(sim.console);
	cpu_delete(sim.cpu);
	window_delete(sim.window);
	terminal_del(sim.terminal);
}

