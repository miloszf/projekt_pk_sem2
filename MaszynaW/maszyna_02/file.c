#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "file.h"
#include "map.h"
#include "error.h"
#include "vector.h"
#include "instruction.h"

#define STR(x) #x
#define TO_STRING(x) STR(x)
#define BUFFER_SIZE 256

#define FIRST_TOKENS_NUMBER 4
#define MAX_FIRST_TOKEN_LENGTH (4 + 1)

struct FileHandler
{
	struct Vector* instruction_lines_vect;
};

struct FileHandler* file_handler_init()
{
	struct FileHandler* new_handler = malloc_s(sizeof(struct FileHandler));
	*new_handler = (struct FileHandler){ NULL };
	return new_handler;
}

void file_handler_delete(struct FileHandler* handler)
{
	if (handler)
	{
		if (handler->instruction_lines_vect)
		{
			char** line_ptr;
			while (line_ptr = vector_pop(handler->instruction_lines_vect))
			{
				free(*line_ptr);
				free(line_ptr);
			}	
			vector_delete(handler->instruction_lines_vect);
		}
		free(handler);
	}
}

void put_to_buffer(unsigned char* buffer, int* buffer_size, int chr, int offset)
{
	CHECK_IF_NULL(buffer);
	CHECK_IF_NULL(buffer_size);

	if (offset >= *buffer_size)
	{
		*buffer_size += BUFFER_SIZE;
		buffer = realloc_s(buffer, *buffer_size);
	}
	buffer[offset] = (unsigned char)chr;
}

inline unsigned char is_whitespace(int chr)
{
	return (chr > ' ' && chr <= 255) ? 0 : 255;
}

int to_lower_latin(int chr)
{
	//chr = tolower(chr);
	if (chr >= 'A' && chr <= 'Z')
		chr += 'a' - 'A';

	//�󹜳���� - windows 1250
	if (chr >= 128)
		switch (chr)
		{
		case 0xca:
		case 0xea:
			chr = 'e';
			break;
		case 0xd3:
		case 0xf3:
			chr = 'o';
			break;
		case 0xa5:
		case 0xb9:
			chr = 'a';
			break;
		case 0x8c:
		case 0x9c:
			chr = 's';
			break;
		case 0xa3:
		case 0xb3:
			chr = 'l';
			break;
		case 0xaf:
		case 0xbf:
			chr = 'z';
			break;
		case 0x8f:
		case 0x9f:
			chr = 'z';
			break;
		case 0xc6:
		case 0xe6:
			chr = 'c';
			break;
		case 0xd1:
		case 0xf1:
			chr = 'n';
			break;
		}
	return chr;
}

struct Vector* get_instruction_lines(const char* file_name)
{
	CHECK_IF_NULL(file_name);
	struct Vector* lines_vect = NULL;

	FILE* input_file;
	if (fopen_s(&input_file, file_name, "r"))
		instr_error_set(NO_FILE, file_name);
	else
	{
		lines_vect = vector_init(sizeof(unsigned char*));
		int last_chr = '\n';
		int string_len = 0;
		int buffer_len = 0;
		int lines_counter = 0;
		unsigned char* line = NULL;

		while (last_chr != EOF)
		{
			int chr = fgetc(input_file);
			if (!is_whitespace(chr))
			{
				if (last_chr == '\n')
				{
					buffer_len = BUFFER_SIZE;
					string_len = 0;
					lines_counter++;
					line = malloc_s(BUFFER_SIZE);
					vector_push(lines_vect, &line);
				}
				
				if (chr == ';' && !is_whitespace(last_chr))
					put_to_buffer(line, &buffer_len, ' ', string_len++);
				put_to_buffer(line, &buffer_len, to_lower_latin(chr), string_len++);
				last_chr = chr;
			}
			else
			{
				if (chr == EOF)
				{
					if (!is_whitespace(last_chr))
					{
						put_to_buffer(line, &buffer_len, '\n', string_len++);
						put_to_buffer(line, &buffer_len, '\0', string_len);
					}
					else if (last_chr != '\n')
					{
						put_to_buffer(line, &buffer_len, '\n', string_len - 1);
						put_to_buffer(line, &buffer_len, '\0', string_len);
					}
					last_chr = chr;
				}
				else if (chr == '\n')
				{
					if (!is_whitespace(last_chr))
					{
						put_to_buffer(line, &buffer_len, '\n', string_len++);
						put_to_buffer(line, &buffer_len, '\0', string_len++);
					}
					else if (last_chr != '\n')
					{
						put_to_buffer(line, &buffer_len, '\n', string_len - 1);
						put_to_buffer(line, &buffer_len, '\0', string_len++);
					}
					last_chr = chr;
				}
				else if (!is_whitespace(last_chr))
				{
					put_to_buffer(line, &buffer_len, ' ', string_len++);
					last_chr = chr;
				}
			}
		}
		fclose(input_file);
	}

	return lines_vect;
}

unsigned int find_header(struct Vector* lines_vect, const char* name)
{
	CHECK_IF_NULL(lines_vect);
	int vect_size = vector_size(lines_vect);
	int line_index = 0;
	for (; line_index < vect_size; line_index++)
	{
		unsigned char** line_ptr = vector_read(lines_vect, line_index);
		if (!line_ptr)
		{
			line_index = EOF;
			break;
		}

		unsigned char buffer[BUFFER_SIZE + 1] = "";
		int read = sscanf_s(*line_ptr, "[%"TO_STRING(BUFFER_SIZE)"[^]]]\n", buffer, BUFFER_SIZE + 1);
		if (read == EOF)
		{
			line_index = EOF;
			break;
		}	
		else if (read == 1 && !strcmp(name, buffer))
		{
			line_index++;
			break;
		}
	}
	return (line_index < vect_size) ? line_index : EOF;
}

bool file_import_setup(const char* file_name, struct FileHandler* handler, struct Map* pref_map)
{
	CHECK_IF_NULL(file_name);
	CHECK_IF_NULL(handler);
	CHECK_IF_NULL(pref_map);

	if (handler->instruction_lines_vect)
	{
		vector_delete(handler->instruction_lines_vect);
		handler->instruction_lines_vect = NULL;
	}

	struct Vector* lines_vect = get_instruction_lines(file_name);
	if (!lines_vect)
		return false;
	else
		handler->instruction_lines_vect = lines_vect;

	const char settings_str[] = "opcje";
	int line_index = find_header(lines_vect, settings_str);
	if (line_index == EOF)
		instr_error_set(MISSING_INPUT, settings_str);

	unsigned char** line_ptr;
	while (!error() && (line_ptr = vector_read(lines_vect, line_index)))
	{
		unsigned char buffer[BUFFER_SIZE + 1];
		unsigned int value;
		int read = sscanf_s(*line_ptr, "%"TO_STRING(BUFFER_SIZE)"[^=]=%u\n", buffer, BUFFER_SIZE + 1, &value);
		if (read == EOF)
			instr_error_set(INVALID_INPUT, *line_ptr);
		else if (read != 2)
			break;
		else
		{
			unsigned char** value_ptr = map_read_from_key(pref_map, buffer);
			if (!value_ptr || !(*value_ptr))
				instr_error_set(INVALID_INPUT, buffer);
			else
			{
				**value_ptr = value;
				line_index++;
			}	
		}
	}
	return !error();
}

struct Vector* get_instr_names(struct FileHandler* handler)
{
	CHECK_IF_NULL(handler);

	struct Vector* instr_vect = vector_init(sizeof(struct Instruction*));
	const char instr_str[] = "rozkazy";

	unsigned int line_index = find_header(handler->instruction_lines_vect, instr_str);
	if (line_index == EOF)
		instr_error_set(MISSING_INPUT, instr_str);
	else
	{
		const char** line_ptr = vector_read(handler->instruction_lines_vect, line_index++);
		if (!line_ptr || !*line_ptr)
			instr_error_set(MISSING_LINE, instr_str);
		else
		{
			unsigned instr_num;
			int read = sscanf_s(*line_ptr, "liczba=%u\n", &instr_num);
			if (read != 1)
				instr_error_set(INVALID_INPUT, *line_ptr);
			else
			{
				for (unsigned i = 0; i < instr_num && !error(); i++)
				{
					line_ptr = vector_read(handler->instruction_lines_vect, line_index++);
					if (!line_ptr || !*line_ptr)
						instr_error_set(MISSING_LINE, instr_str);
					else
					{
						unsigned temp;
						const size_t MAX_NAME_LENGTH = (strlen(*line_ptr));
						char* name = malloc_s(MAX_NAME_LENGTH);
						int read = sscanf_s(*line_ptr, "nazwa%u=%s\n", &temp, name, MAX_NAME_LENGTH);
						if (read != 2)
							instr_error_set(INVALID_INPUT, *line_ptr);
						else
						{
							struct Instruction* new_instr = instruction_init(name);
							vector_push(instr_vect, &new_instr);
						}
						free(name);
					}
				}
			}
		}
	}

	if (error())
	{
		struct Instruction** instr_ptr;
		while (instr_ptr = vector_pop(instr_vect))
		{
			instruction_delete(*instr_ptr);
			free(instr_ptr);
		}
		vector_delete(instr_vect);
		instr_vect = NULL;
	}

	return instr_vect;
}

unsigned count_tick_nodes(struct Tick* tick)
{
	if (tick)
	{
		unsigned l_count = (tick == tick->next) ? 0 : count_tick_nodes(tick->next);
		unsigned r_count = (tick == tick->next_if_true) ? 0 : count_tick_nodes(tick->next_if_true);
		return 1 + l_count + r_count;
	}
	else
		return 0;
}

#define COMPARE(token, str) \
{if (!token || strcmp(token, str))\
{\
	instr_error_set(MISSING_INPUT, str);\
	break;\
}}

struct Vector* file_compile_instructions(struct FileHandler* handler, struct Map* signal_map, struct Map* tag_map)
{
	CHECK_IF_NULL(handler);
	CHECK_IF_NULL(signal_map);
	CHECK_IF_NULL(tag_map);

	struct Vector* instr_vect = get_instr_names(handler);
	if (!instr_vect)
		return NULL;
	
	unsigned instr_number = vector_size(instr_vect);
	for (unsigned instr_index = 0; instr_index < instr_number && !error(); instr_index++)
	{
		struct Instruction** instr_ptr = vector_read(instr_vect, instr_index);
		CHECK_IF_NULL(instr_ptr);
		CHECK_IF_NULL(*instr_ptr);

		const char* instr_name = (*instr_ptr)->name;
		int line_index = find_header(handler->instruction_lines_vect, (*instr_ptr)->name);
		char** line_ptr;
		int lines_number = 0;

		if (line_index == EOF)
			instr_error_set(MISSING_INPUT, instr_name);
		else if (!(line_ptr = vector_read(handler->instruction_lines_vect, line_index)) || !*line_ptr)
			instr_error_set(MISSING_LINE, instr_name);
		else
		{
			int read = sscanf_s(*line_ptr, "linie=%u\n", &lines_number);
			if (read != 1 || lines_number <= 0)
				instr_error_set(INVALID_INPUT, *line_ptr);
			else
				line_index++;
		}

		size_t buffer_size = BUFFER_SIZE;
		char* buffer = malloc_s(BUFFER_SIZE);
		*buffer = '\0';

		for (lines_number += line_index; line_index < lines_number && !error(); line_index++)
		{
			if (!(line_ptr = vector_read(handler->instruction_lines_vect, line_index)) || !*line_ptr)
				instr_error_set(MISSING_LINE, instr_name);
			else
			{
				unsigned unused;
				const size_t MAX_LINE_LENGTH = (strlen(*line_ptr)) + 2;
				char* line_buffer = malloc_s(MAX_LINE_LENGTH);
				int read = sscanf_s(*line_ptr, "linia%u=%[^\n]\n", &unused, line_buffer, MAX_LINE_LENGTH);
				if (read < 1)
					instr_error_set(INVALID_INPUT, *line_ptr);
				// do nothing if read == 1
				else if (read == 2)
				{
					const char comment_str[] = "//";
					char* comment_pos = strstr(line_buffer, comment_str);
					if (comment_pos)
						*comment_pos = '\0';

					size_t buffer_str_len = strlen(buffer);
					size_t new_length = buffer_str_len + strlen(line_buffer) + 1;
					if (buffer_size < new_length)
					{
						buffer_size += BUFFER_SIZE * (strlen(line_buffer) / BUFFER_SIZE + 1);
						buffer = realloc_s(buffer, buffer_size);
					}
					put_to_buffer(buffer, &buffer_size, ' ', buffer_str_len++);
					put_to_buffer(buffer, &buffer_size, '\0', buffer_str_len);
					if (strcat_s(buffer, buffer_size, line_buffer))
						CRASH_LOG(LIBRARY_FUNC_FAILURE);
				}
				free(line_buffer);
			}	
		}

		struct ConditionalMap
		{
			struct Tick* tick;
			const char* label_next;
			const char* label_next_if_true;
		};

		if (!error())
		{
			char* token = NULL;
			char* next_token = buffer;
			const char delim[] = " ";
			size_t strmax = buffer_size;
			typedef enum { COMP_END = 0, INSTR_NAME, ARGUMENTS, FIRST_TICK, GET_TOKEN, SEMICOLON, CONDITIONAL, END_OF_INSTR, LABEL, SIGNAL, COMP_CLEANUP } CompilerState;
			CompilerState state = INSTR_NAME;
			struct Map* label_map = map_init(sizeof(struct Tick*));
			struct Vector* conditional_vect = vector_init(sizeof(struct ConditionalMap));
			struct Tick* current_tick = NULL;
			struct Vector* tick_vect = vector_init(sizeof(struct Tick*));

			while (state && !error())
			{
				switch (state)
				{
				case INSTR_NAME:
				{
					token = strtok_s(buffer, delim, &next_token);
					COMPARE(token, "rozkaz");

					token = strtok_s(NULL, delim, &next_token);
					COMPARE(token, instr_name);

					token = strtok_s(NULL, delim, &next_token);
					COMPARE(token, ";");
					state = ARGUMENTS;
				}
				break;
				case ARGUMENTS:
				{
					state = FIRST_TICK;
					const char arg_str[] = "argumenty";
					char* temp_str = malloc_s(BUFFER_SIZE + 1);
					int read = sscanf_s(next_token, "%"TO_STRING(BUFFER_SIZE)"[^ ]", temp_str, BUFFER_SIZE + 1);
					if (read != 1 || strcmp(arg_str, temp_str))
					{
						(*instr_ptr)->arguments = 1;
						free(temp_str);
						break;
					}
					free(temp_str);

					token = strtok_s(NULL, delim, &next_token);
					COMPARE(token, "argumenty");

					token = strtok_s(NULL, delim, &next_token);
					if (!token)
					{
						instr_error_set(MISSING_INPUT, "args_number");
						break;
					}

					int args_num = atoi(token);
					if (args_num < 0 || args_num > MAX_ARGS_NUM)
					{
						instr_error_set(INVALID_INPUT, token);
						break;
					}
					(*instr_ptr)->arguments = args_num;

					token = strtok_s(NULL, delim, &next_token);
					COMPARE(token, ";");
				}
				break;
				case FIRST_TICK:
				{
					current_tick = tick_init();
					vector_push(tick_vect, &current_tick);
					char expected_tokens[FIRST_TOKENS_NUMBER][MAX_FIRST_TOKEN_LENGTH] = { "czyt", "wys", "wei", "il" };
					unsigned tokens_number = FIRST_TOKENS_NUMBER;
					for (unsigned i = 0; i < tokens_number && !error(); i++)
					{
						token = strtok_s(NULL, delim, &next_token);
						if (token)
						{
							bool found = false;
							for (unsigned j = 0; j < tokens_number; j++)
								if (!strcmp(token, expected_tokens[j]))
								{
									(expected_tokens[j])[0] = '\0';
									struct Signal** signal_ptr = map_read_from_key(signal_map, token);
									vector_push(current_tick->signal_vect, signal_ptr);
									found = true;
									break;
								}
							if (!found)
								instr_error_set(INVALID_INPUT, token);
						}
						else
							instr_error_set(MISSING_LINE, instr_name);
					}

					if (!error())
					{
						token = strtok_s(NULL, delim, &next_token);
						COMPARE(token, ";");
					}
					state = SEMICOLON;
				}
				break;
				case GET_TOKEN:
				{
					token = strtok_s(NULL, delim, &next_token);
					if (!token)
						state = COMP_CLEANUP;
					else if (!strcmp(token, ";"))
						state = SEMICOLON;
					else if (!strcmp(token, "jezeli"))
						state = CONDITIONAL;
					else if (!strcmp(token, "koniec"))
						state = END_OF_INSTR;
					else if (*token == '@')
						state = LABEL;
					else
						state = SIGNAL;
				}
				break;
				case SEMICOLON:
				{
					current_tick = tick_init();
					vector_push(tick_vect, &current_tick);
					state = GET_TOKEN;
				}
				break;
				case LABEL:
				{
					if (map_read_from_key(label_map, token))
						instr_error_set(UNKNOWN_LABEL, token);
					else
					{
						map_push(label_map, token, &current_tick);
						state = GET_TOKEN;
					}
				}
				break;
				case CONDITIONAL:
				{
					token = strtok_s(NULL, delim, &next_token);
					void** condition_ptr;
					if (!token)
					{
						instr_error_set(MISSING_INPUT, "tag_name");
						break;
					}
					else if (!(condition_ptr = map_read_from_key(tag_map, token)))
					{
						instr_error_set(INVALID_INPUT, token);
						break;
					}
					current_tick->condition = *condition_ptr;

					token = strtok_s(NULL, delim, &next_token);
					COMPARE(token, "to");

					token = strtok_s(NULL, delim, &next_token);
					if (!token)
					{
						instr_error_set(MISSING_INPUT, "label_name");
						break;
					}
					else if (*token != '@')
					{
						instr_error_set(INVALID_INPUT, token);
						break;
					}
						
					struct ConditionalMap c_map = { .tick = current_tick, .label_next_if_true = token };

					token = strtok_s(NULL, delim, &next_token);
					COMPARE(token, "gdy");

					token = strtok_s(NULL, delim, &next_token);
					COMPARE(token, "nie");

					token = strtok_s(NULL, delim, &next_token);
					if (!token)
					{
						instr_error_set(MISSING_INPUT, "label_name");
						break;
					}
					else if (*token != '@')
					{
						instr_error_set(INVALID_INPUT, token);
						break;
					}

					c_map.label_next = token;
					vector_push(conditional_vect, &c_map);
					token = strtok_s(NULL, delim, &next_token);
					COMPARE(token, ";");
					state = SEMICOLON;
				}
				break;
				case END_OF_INSTR:
				{
					struct ConditionalMap c_map = { .tick = current_tick, NULL, NULL };
					vector_push(conditional_vect, &c_map);
					state = GET_TOKEN;
				}
				break;
				case SIGNAL:
				{
					struct Signal** signal_ptr = map_read_from_key(signal_map, token);
					if (!signal_ptr)
						instr_error_set(INVALID_INPUT, token);
					else
					{
						CHECK_IF_NULL(*signal_ptr);
						vector_push(current_tick->signal_vect, signal_ptr);
						state = GET_TOKEN;
					}
				}
				break;
				case COMP_CLEANUP:
				{
					if (!vector_size(current_tick->signal_vect) && !(current_tick->condition))
					{
						free(vector_pop(tick_vect));
						tick_delete(current_tick);
						current_tick = NULL;
					}
					state = COMP_END;
				}
				break;
				default:
					CRASH_LOG(LOG_UNKNOWN_VALUE);
					break;
				}
			}

			if (!error())
			{
				size_t tick_array_size;
				struct Tick** tick_array = vector_convert_to_array(tick_vect, &tick_array_size);
				tick_vect = NULL;
				CHECK_IF_NULL(tick_array);
				size_t cond_array_size;
				struct ConditionalMap* cond_array = vector_convert_to_array(conditional_vect, &cond_array_size);
				conditional_vect = NULL;
				CHECK_IF_NULL(cond_array);
				unsigned empty_ticks_num = 0;

				for (unsigned tick_index = 0; tick_index < tick_array_size && !error(); tick_index++)
				{
					struct Tick* tick = tick_array[tick_index];
					CHECK_IF_NULL(tick);
					bool found = false;

					for (unsigned j = 0; j < cond_array_size && !found; j++)
					{
						if (cond_array[j].tick == tick)
						{
							if (!cond_array[j].label_next || !cond_array[j].label_next_if_true)
							{
								tick->next = NULL;
								tick->next_if_true = NULL;
							}
							else
							{
								struct Tick** next_tick_ptr = map_read_from_key(label_map, cond_array[j].label_next);
								if (!next_tick_ptr)
								{
									instr_error_set(MISSING_INPUT, cond_array[j].label_next);
									break;
								}		
								tick->next = *next_tick_ptr;
								struct Tick** next_if_true_tick_ptr = map_read_from_key(label_map, cond_array[j].label_next_if_true);
								if (!next_if_true_tick_ptr)
								{
									instr_error_set(MISSING_INPUT, cond_array[j].label_next_if_true);
									break;
								}
								tick->next_if_true = *next_if_true_tick_ptr;
							}
							found = true;
						}
					}

					if (!found && !error())
					{
						if (vector_size(tick->signal_vect))
						{
							if (tick_index + 1 < tick_array_size)
								tick->next = tick_array[tick_index + 1];
							else
								tick->next = NULL;
							tick->next_if_true = NULL;
						}
						else
						{
							tick_delete(tick);
							tick = NULL;
							tick_array[tick_index] = NULL;
							empty_ticks_num++;
						}
					}
				}

				if (!error())
				{
					unsigned connected_ticks_num = count_tick_nodes(*tick_array);
					if (connected_ticks_num != (tick_array_size - empty_ticks_num))
						instr_error_set(LOST_TICK, instr_name);
					else
						(*instr_ptr)->first_tick = *tick_array;
				}
				free(tick_array);
				free(cond_array);
			}
			map_delete(label_map);
			vector_delete(conditional_vect);
			vector_delete(tick_vect);
		}
		free(buffer);
	}

	if (error())
	{
		struct Instruction** instr_ptr;
		while (instr_ptr = vector_pop(instr_vect))
		{
			instruction_delete(*instr_ptr);
			free(instr_ptr);
		}
		vector_delete(instr_vect);
		instr_vect = NULL;
	}
		
	return instr_vect;
}

struct Vector* get_program_lines(const char* file_name)
{
	CHECK_IF_NULL(file_name);
	struct Vector* lines_vect = NULL;

	FILE* input_file;
	if (!fopen_s(&input_file, file_name, "r"))
	{
		lines_vect = vector_init(sizeof(char*));
		int last_chr = '\n';
		int string_len = 0;
		int buffer_len = 0;
		int lines_counter = 0;
		unsigned char* line = NULL;

		while (last_chr != EOF)
		{
			int chr = fgetc(input_file);
			if (last_chr == '\n')
			{
				buffer_len = BUFFER_SIZE;
				string_len = 0;
				lines_counter++;
				line = malloc_s(BUFFER_SIZE);
				vector_push(lines_vect, &line);
			}

			last_chr = chr;
			if (chr == EOF || chr == '\n')
				chr = '\0';
			else if (is_whitespace(chr))
				chr = ' ';
			else
				chr = to_lower_latin(chr);
			put_to_buffer(line, &buffer_len, chr, string_len++);
		}

		fclose(input_file);
	}
	else
		prog_error_set(NO_FILE, file_name);

	return lines_vect;
}

bool file_compile_program(const char* file_name, struct Vector* instr_vect, var addr_len, var code_len, var* memory) 
{
	CHECK_IF_NULL(instr_vect);
	CHECK_IF_NULL(memory);

	struct Vector* lines_vect = get_program_lines(file_name);
	if (!lines_vect)
		return false;

	unsigned lines_number = vector_size(lines_vect);
	var memory_index = 0;
	const var memory_size = 1 << addr_len;
	struct Map* label_map = map_init(sizeof(var));

	struct SoughtLabel
	{
		const char* name;
		var line;
	};
	struct Vector* sought_label_vect = vector_init(sizeof(struct SoughtLabel));

	struct InstructionProperties
	{
		var value_mask;
		var code;
		unsigned char args_num;
	};
	struct Map* instr_map = map_init(sizeof(struct InstructionProperties));
	{
		size_t instr_num = vector_size(instr_vect);
		var value_mask = (u_var)(-1) >> (sizeof(var) * 8 - addr_len);
		for (unsigned i = 0; i < instr_num; i++)
		{
			struct Instruction** instr_ptr = vector_read(instr_vect, i);
			CHECK_IF_NULL(instr_ptr);
			CHECK_IF_NULL(*instr_ptr);
			struct InstructionProperties instr_prop = { value_mask, i << addr_len, (*instr_ptr)->arguments };
			map_push(instr_map, (*instr_ptr)->name, &instr_prop);
		}
		
		value_mask = (u_var)(-1) >> (sizeof(var) * 8 - (addr_len + code_len));
		struct InstructionProperties instr_prop = { value_mask, 0, 0 };
		map_push(instr_map, "rpa", &instr_prop);
		instr_prop.args_num = 1;
		map_push(instr_map, "rst", &instr_prop);
	}
	
	for (unsigned line_index = 0; line_index < lines_number && !error(); line_index++)
	{
		char** line_ptr = vector_read(lines_vect, line_index);
		CHECK_IF_NULL(line_ptr);

		const char comment_str[] = "//";
		char* comment_pos = strstr(*line_ptr, comment_str);
		if (comment_pos)
			*comment_pos = '\0';

		char* line = *line_ptr;
		CHECK_IF_NULL(line);

		char* token = NULL;
		char* next_token = line;
		const char delim[] = " ";
		struct Vector* token_vect = vector_init(sizeof(char*));
		unsigned tokens_read = 0;
		
		if (token = strtok_s(line, delim, &next_token))
		{
			unsigned tokens_read = 0;
			do
			{
				tokens_read++;
				vector_push(token_vect, &token);
			} while (token = strtok_s(NULL, delim, &next_token));

			size_t token_array_size;
			char** token_array = vector_convert_to_array(token_vect, &token_array_size);
			token_vect = NULL;
			unsigned token_index = 0;
			
			size_t first_token_len = strlen(token_array[0]);
			if (!first_token_len)
				prog_error_set(INVALID_INPUT, token_array[0]);
			else if ((token_array[0])[first_token_len - 1] == ':')
				{
					(token_array[0])[first_token_len - 1] = '\0';
					token_index++;
					// label already exist
					if (!map_push(label_map, token_array[0], &memory_index))
						prog_error_set(REPEATED_LABEL, token_array[0]);
				}
			
			if (!error() && token_index < token_array_size)
			{
				struct InstructionProperties* instr_prop = map_read_from_key(instr_map, token_array[token_index]);
				if (!instr_prop || instr_prop->args_num != (token_array_size - 1 - token_index))
					prog_error_set(INVALID_INPUT, token_array[token_index]);
				else
				{
					if (memory_index >= memory_size)
						prog_error_set(FULL_MEMORY, NULL);
					else
					{
						memory[memory_index] = instr_prop->code;
						if (!instr_prop->args_num)
							memory_index++;
						else
							for (token_index++; token_index < token_array_size && !error(); token_index++)
							{
								if (memory_index >= memory_size)
									prog_error_set(FULL_MEMORY, NULL);
								else
								{
									char* end;
									long number = strtol(token_array[token_index], &end, 0);
									// for a value/address given as a number
									if (end != token_array[token_index])
										memory[memory_index] |= (number & instr_prop->value_mask);
									// for a value given as a character
									else
									{
										char chr;
										int read = sscanf_s(token_array[token_index], "'%c'", &chr, sizeof(chr));
										if (read == 1)
											memory[memory_index] |= (chr & instr_prop->value_mask);
										// for an address given as a label
										else
										{
											struct SoughtLabel s_label = { token_array[token_index], memory_index };
											vector_push(sought_label_vect, &s_label);
										}
									}
									memory_index++;
								}
							}
					}
				}
			}
			free(token_array);
		}
		vector_delete(token_vect);
	}

	struct SoughtLabel* s_label_ptr;
	while (!error() && (s_label_ptr = vector_pop(sought_label_vect)))
	{
		var* label_index_ptr = map_read_from_key(label_map, s_label_ptr->name);
		if (!label_index_ptr)
			prog_error_set(MISSING_INPUT, s_label_ptr->name);
		//	nieistniej�ca etykieta
		else
			memory[s_label_ptr->line] |= *label_index_ptr;
		free(s_label_ptr);
	}

	map_delete(instr_map);
	map_delete(label_map);
	vector_delete(sought_label_vect);

	char** line_ptr;
	while (line_ptr = vector_pop(lines_vect))
	{
		free(*line_ptr);
		free(line_ptr);
	}
	vector_delete(lines_vect);

	return !error();
}