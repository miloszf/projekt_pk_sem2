#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "file.h"
#include "map.h"
#include "error.h"
#include "vector.h"
#include "instruction.h"

//DEBUG
#include "signal.h"

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
	//new_handler->instruction_lines_vect = vector_init(sizeof(unsigned char*));
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
				free(*line_ptr);
			vector_delete(handler->instruction_lines_vect);
		}
			
		free(handler);
	}
}

void put_to_buffer(unsigned char* buffer, int* buffer_size, int chr, int offset)
{
	check_for_NULL(buffer);
	check_for_NULL(buffer_size);

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

	//êó¹œ³¿Ÿæñ - windows 1250
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

void dump_lines(struct Vector* line_vect)
{
	check_for_NULL(line_vect);
	
	unsigned vect_size = vector_size(line_vect);
	for (unsigned i = 0; i < vect_size; i++)
	{
		const char** line_ptr = vector_read(line_vect, i);
		if (line_ptr && *line_ptr)
			printf("%2u - %s", i, *line_ptr);
	}
}

struct Vector* get_lines(const char* file_name)
{
	check_for_NULL(file_name);

	Error error = NO_ERROR;
	struct Vector* lines_vect = NULL;

	//FILE* input_file = fopen(file_name, "r");
	FILE* input_file;
	if (fopen_s(&input_file, file_name, "r"))
		error = ERROR_NO_INSTR_FILE;
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
			char DEBUG = chr;
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
				{
					put_to_buffer(line, &buffer_len, ' ', string_len++);
				}
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

	if (error)
		error_set(error);

	return lines_vect;
}

unsigned int find_header(struct Vector* lines_vect, const char* name)
{
	check_for_NULL(lines_vect);
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

bool file_import_setup(const char* file_name, struct FileHandler* files_handler, struct Map* pref_map)
{
	check_for_NULL(file_name);
	check_for_NULL(files_handler);
	check_for_NULL(pref_map);

	if (files_handler->instruction_lines_vect)
	{
		vector_delete(files_handler->instruction_lines_vect);
		files_handler->instruction_lines_vect = NULL;
	}

	struct Vector* lines_vect = get_lines(file_name);
	if (!lines_vect)
		return false;
	else
		files_handler->instruction_lines_vect = lines_vect;

	Error error = NO_ERROR;
	const char settings_str[] = "opcje";
	int line_index = find_header(lines_vect, settings_str);
	if (line_index == EOF)
		error = ERROR_INVALID_INSTR_FILE;

	unsigned char** line_ptr;
	while (!error && (line_ptr = vector_read(lines_vect, line_index)))
	{
		unsigned char buffer[BUFFER_SIZE + 1];
		unsigned int value;
		int read = sscanf_s(*line_ptr, "%"TO_STRING(BUFFER_SIZE)"[^=]=%u\n", buffer, BUFFER_SIZE + 1, &value);
		if (read == EOF)
			error = ERROR_INVALID_INSTR_FILE;
		else if (read != 2)
			break;
		else
		{
			unsigned char** value_ptr = map_read_from_key(pref_map, buffer);
			if (!value_ptr || !(*value_ptr))
				error = ERROR_INVALID_INSTR_FILE;
			else
			{
				**value_ptr = value;
				line_index++;
			}	
		}
	}

	if (error)
		error_set(error);

	return !error;
}

struct Vector* get_instr_names(struct FileHandler* handler)
{
	check_for_NULL(handler);

	struct Vector* instr_vect = vector_init(sizeof(struct Instruction*));
	Error error = NO_ERROR;
	const char instr_str[] = "rozkazy";

	unsigned int line_index = find_header(handler->instruction_lines_vect, instr_str);
	if (line_index == EOF)
		error = ERROR_INVALID_INSTR_FILE;
	else
	{
		const char** line_ptr = vector_read(handler->instruction_lines_vect, line_index++);
		if (!line_ptr || !*line_ptr)
			error = ERROR_INVALID_INSTR_FILE;
		else
		{
			unsigned instr_num;
			int read = sscanf_s(*line_ptr, "liczba=%u\n", &instr_num);
			if (read != 1)
				error = ERROR_INVALID_INSTR_FILE;
			else
			{
				for (unsigned i = 0; i < instr_num && !error; i++)
				{
					line_ptr = vector_read(handler->instruction_lines_vect, line_index++);
					if (!line_ptr || !*line_ptr)
						error = ERROR_INVALID_INSTR_FILE;
					else
					{
						unsigned temp;
						const size_t MAX_NAME_LENGTH = (strlen(*line_ptr));
						char* name = malloc_s(MAX_NAME_LENGTH);
						int read = sscanf_s(*line_ptr, "nazwa%u=%s\n", &temp, name, MAX_NAME_LENGTH);
						if (read != 2)
							error = ERROR_INVALID_INSTR_FILE;
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

	if (error)
	{
		struct Instruction** instr_ptr;
		while (instr_ptr = vector_pop(instr_vect))
			instruction_delete(*instr_ptr);
		vector_delete(instr_vect);
		instr_vect = NULL;
		error_set(error);
	}

	return instr_vect;
}

//void* find_and_erease(void* array, size_t array_size, void* value)
//{
//	if (!array || !value || !array_size)
//		return NULL;
//
//
//}

unsigned count_tick_nodes(struct Tick* tick)
{
	return (tick) ? 1 + count_tick_nodes(tick->next) + count_tick_nodes(tick->next_if_true) : 0;
}

struct Vector* file_compile_instructions(struct FileHandler* handler, struct Map* signal_map, struct Map* tag_map)
{
	check_for_NULL(handler);
	check_for_NULL(signal_map);
	check_for_NULL(tag_map);

	//struct Vector* instr_vect = NULL;
	struct Vector* instr_vect = get_instr_names(handler);
	if (!instr_vect)
		return NULL;

	Error error = NO_ERROR;
	//printf("Poczatek file_compile_instructions: \n");
	//dump_lines(handler->instruction_lines_vect);
	//printf("\nOdczytane rozkazy: \n");

	
	unsigned instr_number = vector_size(instr_vect);
	for (unsigned instr_index = 0; instr_index < instr_number && !error; instr_index++)
	{
		struct Instruction** instr_ptr = vector_read(instr_vect, instr_index);
		check_for_NULL(instr_ptr);
		check_for_NULL(*instr_ptr);
		// DEBUG
		printf("%u - '%s'\n", instr_index, (*instr_ptr)->name);
		const char* instr_name = (*instr_ptr)->name;

		unsigned line_index = find_header(handler->instruction_lines_vect, (*instr_ptr)->name);
		char** line_ptr;
		unsigned lines_number = 0;

		if (line_index == EOF || !(line_ptr = vector_read(handler->instruction_lines_vect, line_index)) || !*line_ptr)
			error = ERROR_INVALID_INSTR_FILE;
		else
		{
			int read = sscanf_s(*line_ptr, "linie=%u\n", &lines_number);
			if (read != 1)
				error = ERROR_INVALID_INSTR_FILE;
			else
				line_index++;
		}

		//int string_len = 0;
		size_t buffer_size = BUFFER_SIZE;
		char* buffer = malloc_s(BUFFER_SIZE);
		*buffer = '\0';

		for (lines_number += line_index; line_index < lines_number && !error; line_index++)
		{
			if (!(line_ptr = vector_read(handler->instruction_lines_vect, line_index)) || !*line_ptr)
				error = ERROR_INVALID_INSTR_FILE;
			else
			{
				// DEBUG
				printf("%u - %s", line_index, *line_ptr);
				unsigned unused;
				const size_t MAX_LINE_LENGTH = (strlen(*line_ptr)) + 2;
				char* line_buffer = malloc_s(MAX_LINE_LENGTH);
				int read = sscanf_s(*line_ptr, "linia%u=%[^\n]", &unused, line_buffer, MAX_LINE_LENGTH);
				if (read != 2)
					error = ERROR_INVALID_INSTR_FILE;
				else
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
						error = ERROR_STRING_HANDLING;
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

		if (!error)
		{
			printf("Odczytano: '%s'\n", buffer);

			char* token = NULL;
			char* next_token = buffer;
			const char delim[] = " ";
			size_t strmax = buffer_size;
			//token = strtok_s(buffer, delim, &next_token);
			typedef enum { COMP_END = 0, INSTR_NAME, ARGUMENTS, FIRST_TICK, GET_TOKEN, SEMICOLON, CONDITIONAL, END_OF_INSTR, LABEL, SIGNAL, COMP_CLEANUP } CompilerState;
			CompilerState state = INSTR_NAME;
			//struct Tick** tick_ptr = NULL;
			struct Map* label_map = map_init(sizeof(struct Tick*));
			struct Vector* conditional_vect = vector_init(sizeof(struct ConditionalMap));
			//struct Tick* first_tick = NULL;
			struct Tick* current_tick = NULL;
			struct Vector* tick_vect = vector_init(sizeof(struct Tick*));

			while (state && !error)
			{
				switch (state)
				{
				case INSTR_NAME:
				{
					//state = COMP_ERROR;
					error = ERROR_INSTR_COMPILATION;
					token = strtok_s(buffer, delim, &next_token);
					if (!token || strcmp(token, "rozkaz"))
						break;
					token = strtok_s(NULL, delim, &next_token);
					if (!token || strcmp(token, instr_name))
						break;
					token = strtok_s(NULL, delim, &next_token);
					if (!token || strcmp(token, ";"))
						break;
					state = ARGUMENTS;
					error = NO_ERROR;
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

					error = ERROR_INSTR_COMPILATION;
					token = strtok_s(NULL, delim, &next_token);
					// potrzebne?
					if (!token || strcmp(token, "argumenty"))
						break;
					token = strtok_s(NULL, delim, &next_token);
					if (!token)
						break;
					int args_num = atoi(token);
					if (args_num < 0 || args_num > MAX_ARGS_NUM)
						break;
					(*instr_ptr)->arguments = args_num;
					token = strtok_s(NULL, delim, &next_token);
					if (!token || strcmp(token, ";"))
						break;
					error = NO_ERROR;
				}
				break;
				case FIRST_TICK:
				{
					//state = COMP_ERROR;

					Error local_error = NO_ERROR;
					current_tick = tick_init();
					vector_push(tick_vect, &current_tick);
					char expected_tokens[FIRST_TOKENS_NUMBER][MAX_FIRST_TOKEN_LENGTH] = { "czyt", "wys", "wei", "il" };
					unsigned tokens_number = FIRST_TOKENS_NUMBER;
					//char* read_token;
					for (unsigned i = 0; i < tokens_number && !local_error; i++)
					{
						local_error = ERROR;
						token = strtok_s(NULL, delim, &next_token);
						if (token)
							for (unsigned j = 0; j < tokens_number; j++)
								if (!strcmp(token, expected_tokens[j]))
								{
									local_error = NO_ERROR;
									(expected_tokens[j])[0] = '\0';
									struct Signal** signal_ptr = map_read_from_key(signal_map, token);
									vector_push(current_tick->signal_vect, signal_ptr);
									break;
								}
					}

					if (!local_error)
					{
						token = strtok_s(NULL, delim, &next_token);
						if (!token || strcmp(token, ";"))
							local_error = ERROR;
					}

					if (!local_error)
						state = SEMICOLON;
					else
						error = ERROR_INSTR_COMPILATION;
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
						error = ERROR_INSTR_COMPILATION;
					else
					{
						map_push(label_map, token, &current_tick);
						state = GET_TOKEN;
					}
				}
				break;
				case CONDITIONAL:
				{
					error = ERROR_INSTR_COMPILATION;
					token = strtok_s(NULL, delim, &next_token);
					bool** condition_ptr;
					if (!token || !(condition_ptr = map_read_from_key(tag_map, token)))
						break;
					current_tick->condition = *condition_ptr;

					token = strtok_s(NULL, delim, &next_token);
					if (!token || strcmp(token, "to"))
						break;

					token = strtok_s(NULL, delim, &next_token);
					if (!token || *token != '@')
						break;
					struct ConditionalMap c_map = { .tick = current_tick, .label_next = token };

					token = strtok_s(NULL, delim, &next_token);
					if (!token || strcmp(token, "gdy"))
						break;
					token = strtok_s(NULL, delim, &next_token);
					if (!token || strcmp(token, "nie"))
						break;

					token = strtok_s(NULL, delim, &next_token);
					if (!token || *token != '@')
						break;
					c_map.label_next_if_true = token;
					vector_push(conditional_vect, &c_map);
					token = strtok_s(NULL, delim, &next_token);
					if (!token || strcmp(token, ";"))
						break;
					state = SEMICOLON;
					error = NO_ERROR;
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
						error = ERROR_INSTR_COMPILATION;
					else
					{
						check_for_NULL(*signal_ptr);
						vector_push(current_tick->signal_vect, signal_ptr);
						state = GET_TOKEN;
					}
				}
				break;
				case COMP_CLEANUP:
				{
					if (!vector_size(current_tick->signal_vect) && !(current_tick->condition))
					{
						vector_pop(tick_vect);
						tick_delete(current_tick);
						current_tick = NULL;
					}
					state = COMP_END;
				}
				break;
				default:
					error = ERROR;
				}
			}

			if (!error)
			{
				size_t tick_array_size;
				struct Tick** tick_array = vector_unwrap(tick_vect, &tick_array_size);
				tick_vect = NULL;
				check_for_NULL(tick_array);
				size_t cond_array_size;
				struct ConditionalMap* cond_array = vector_unwrap(conditional_vect, &cond_array_size);
				conditional_vect = NULL;
				check_for_NULL(cond_array);
				unsigned empty_ticks_num = 0;

				for (unsigned tick_index = 0; tick_index < tick_array_size && !error; tick_index++)
				{
					struct Tick* tick = tick_array[tick_index];
					check_for_NULL(tick);
					bool found = false;
					// DEBUG
					printf("Takt nr %u: '", tick_index);
					for (unsigned i = 0; i < vector_size(tick->signal_vect); i++)
					{
						struct Signal** sig_ptr = vector_read(tick->signal_vect, i);
						printf("%s,", signal_get_name(*sig_ptr));
					}
					printf("'\n");
					// /DEBUG

					for (unsigned j = 0; j < cond_array_size && !found; j++)
					{
						struct ConditionalMap debug = cond_array[j];
						if (cond_array[j].tick == tick)
						{
							if (!cond_array[j].label_next || !cond_array[j].label_next_if_true)
							{
								tick->next = NULL;
								tick->next_if_true = NULL;
							}
							else
							{
								error = ERROR_INSTR_COMPILATION;
								struct Tick** next_tick_ptr = map_read_from_key(label_map, cond_array[j].label_next);
								if (!next_tick_ptr)
									break;
								tick->next = *next_tick_ptr;
								struct Tick** next_if_true_tick_ptr = map_read_from_key(label_map, cond_array[j].label_next_if_true);
								if (!next_if_true_tick_ptr)
									break;
								tick->next_if_true = *next_if_true_tick_ptr;
								error = NO_ERROR;
							}
							found = true;
						}
					}

					if (!found && !error)
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

				if (!error)
				{
					unsigned connected_ticks_num = count_tick_nodes(*tick_array);
					if (connected_ticks_num != (tick_array_size - empty_ticks_num))
						error = ERROR_INSTR_COMPILATION;
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

	if (error)
	{
		struct Instruction** instr_ptr;
		while (instr_ptr = vector_pop(instr_vect))
			instruction_delete(*instr_ptr);
		vector_delete(instr_vect);
		instr_vect = NULL;
		error_set(error);
	}
		
	return instr_vect;
}