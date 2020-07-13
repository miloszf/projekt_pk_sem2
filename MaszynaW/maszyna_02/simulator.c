#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "simulator.h"
#include "cpu.h"
#include "terminal.h"
#include "window.h"
#include "graphics.h"
#include "event.h"
#include "error.h"
#include "settings.h"
#include "console.h"

#define INTERRUPTS_NUMBER 4

struct Simulator
{
	struct Terminal* terminal;
	struct Window* window;
	struct CPU* cpu;
	struct Console* err_console;
	struct Console* in_console;
	struct Console* out_console;
};

typedef enum { READY, INIT, EXIT, CLEAR, TICK, INSTR, PROGRAM, RELOAD_INSTR, RELOAD_PROGRAM} SimState;

struct UserInput
{
	SimState new_state;
	var mouse_scroll;
	char interrupts;
	char chr;
};

struct UserInput simulator_get_events(struct Vector* events_vect)
{
	CHECK_IF_NULL(events_vect);
	struct UserInput new_input = { READY, 0, 0, '\0' };
	struct InputEvent* event_ptr;
	while ((event_ptr = vector_pop(events_vect)) && new_input.new_state != EXIT)
	{
		if (event_ptr->type == EVENT_KEY && event_ptr->key_event.key_down)
		{
			if (event_ptr->key_event.key == ESC_KEY)
				new_input.new_state = EXIT;
			else if (event_ptr->key_event.key >= F1_KEY && event_ptr->key_event.key <= F4_KEY)
				new_input.interrupts |= 1 << (INTERRUPTS_NUMBER - (event_ptr->key_event.key - F1_KEY + 1));
			else if (event_ptr->key_event.key >= ' ' && event_ptr->key_event.key < CONTROL_KEY)
				new_input.chr = event_ptr->key_event.key;
			else switch (event_ptr->key_event.key)
			{
			case F5_KEY:
				new_input.new_state = CLEAR;
				break;
			case F6_KEY:
				new_input.new_state = TICK;
				break;
			case F7_KEY:
				new_input.new_state = INSTR;
				break;
			case F8_KEY:
				new_input.new_state = PROGRAM;
				break;
			case F9_KEY:
				new_input.new_state = RELOAD_INSTR;
				break;
			case F10_KEY:
				new_input.new_state = RELOAD_PROGRAM;
				break;
			case ENTER_KEY:
				new_input.chr = '\n';
				break;
			default:
				break;
			}
		}
		else if (event_ptr->type == EVENT_MOUSE) 
			new_input.mouse_scroll += event_ptr->mouse_event.scroll;
		free(event_ptr);
	}
	vector_delete(events_vect);
	return new_input;
}

struct CPU_IO_Token
{
	struct Console* console;
};

char simulator_console_to_cpu(struct CPU_IO_Token* token)
{
	int chr = console_get_char(token->console);
	return (chr == EOL) ? 0 : (char)chr;
}

void simulator_cpu_to_console(struct CPU_IO_Token* token, char chr)
{
	if (chr == 10 || chr == 13)
		chr = '\n';
	char str[] = { chr, '\0' };
	console_print(token->console, str);
}

void simulator_run(int argc, char** argv)
{
	init_crash_log();
	struct Simulator sim = { NULL, NULL, NULL, NULL };

	sim.terminal = terminal_init("MaszynaW");
	sim.window = window_init();
	struct Canvas* cpu_canvas = window_new_canvas(sim.window, (Point) { 0, 0 }, (Point) { 73, 31 });
	struct Canvas* err_console_canvas = window_new_canvas(sim.window, (Point) { 72, 0 }, (Point) { 40, 16 });
	struct Canvas* io_console_canvas = window_new_canvas(sim.window, (Point) { 72, 15 }, (Point) { 40, 16 });
	sim.err_console = console_init(err_console_canvas, POINT(0, 0), POINT(40, 16)); // 40, 16
	//sim.err_console = console_init(err_console_canvas, POINT(0, 0), POINT(12, 5)); // 40, 16
	drawable_new_frame(io_console_canvas, POINT(0, 0), POINT(40, 16));
	sim.in_console = console_init(io_console_canvas, POINT(1, 1), POINT(38, 4)); // POINT(38, 3)
	sim.out_console = console_init(io_console_canvas, POINT(1, 4), POINT(38, 11)); // POINT(38, 12)
	struct CPU_IO_Token* in_token = malloc_s(sizeof(struct CPU_IO_Token));
	*in_token = (struct CPU_IO_Token){ sim.in_console };
	struct CPU_IO_Token* out_token = malloc_s(sizeof(struct CPU_IO_Token));
	*out_token = (struct CPU_IO_Token){ sim.out_console };
	struct CPU_IO_Handler cpu_io_handler = { &simulator_console_to_cpu, in_token, &simulator_cpu_to_console, out_token };
	sim.cpu = cpu_init(cpu_canvas, cpu_io_handler);

	SimState state = INIT;

	// DEBUG
	const char instr_file_name[] = "tests/MaszynaW.lst";
	const char prog_file_name[] = "tests/test_10.prg";

	while (state != EXIT)
	{
		bool redraw = false;
		bool log_error = false;
		struct UserInput user_input = simulator_get_events(event_get(sim.terminal));
		if (user_input.new_state == EXIT)
			state = EXIT;
		else
		{
			//console_print
			cpu_user_input_set(sim.cpu, user_input.mouse_scroll, user_input.interrupts);
		}
		
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
			//state = EXIT;
		}
		break;
		case READY:
		{
			if (user_input.new_state != READY)
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
			//state = EXIT;
		}
		break;
		case EXIT:
			break;
		case RELOAD_INSTR:
		{
			error_reset();
			if (!cpu_import_instructions(sim.cpu, instr_file_name) || !cpu_import_program(sim.cpu, prog_file_name))
			{
				log_error = true;
				state = READY;
			}
			else
				state = CLEAR;
			redraw = true;
		}
		break;
		case RELOAD_PROGRAM:
		{
			error_reset();
			if (!cpu_import_program(sim.cpu, prog_file_name))
			{
				log_error = true;
				state = READY;
			}
			else
				state = CLEAR;
			redraw = true;
		}
		break;
		case CLEAR:
		{
			error_reset();
			cpu_reset(sim.cpu);
			console_clear(sim.err_console);
			console_clear(sim.in_console);
			console_clear(sim.out_console);
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

		if (user_input.chr)
		{
			char str[] = { user_input.chr, '\0' };
			if (user_input.chr == '`')
			{
				int chr = console_get_char(sim.in_console);
				if (chr != EOL)
				{
					*str = chr;
					console_print(sim.out_console, str);
				}	
			}
			else
			{
				console_print(sim.in_console, str);
				//console_print(sim.err_console, str);
			}
		}

		if (log_error)
		{
			console_print(sim.err_console, error_msg());
			log_error = false;
		}

		redraw = true;
		if (redraw)
		{
			window_draw(sim.window);
			terminal_display(sim.terminal, sim.window);
			/* redraw = false; */
		}
	}

	console_delete(sim.err_console);
	console_delete(sim.in_console);
	console_delete(sim.out_console);
	cpu_delete(sim.cpu);
	window_delete(sim.window);
	terminal_del(sim.terminal);
	_debug_error_delete();
}

