#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#pragma warning(disable : 4996)

#include "file_handling.h"
#include "error.h"

#define STR(x) #x
#define TO_STRING(x) STR(x)
#define BUFFER_SIZE 256
#define INSTRUCTIONS_STR "rozkazy"

Error put_to_buffer(unsigned char* buffer, int* buffer_size, int chr, int offset)
{
	if (!buffer || !buffer_size)
		return ERROR_NULL_POINTER;

	Error error = NO_ERROR;
	if (offset >= *buffer_size)
	{
		*buffer_size += BUFFER_SIZE;
		char* temp = realloc(buffer, *buffer_size);
		if (!temp)
			error = ERROR_MALLOC_FAILURE;
		else
			buffer = temp;
	}

	if (!error)
		buffer[offset] = (unsigned char)chr;

	return error;
}

inline unsigned char is_whitespace(int chr)
{
	return (chr > ' ' && chr <= 255) ? 0 : 255;
}

int to_lower_latin(int chr)
{
	chr = tolower(chr);
	//êó¹œ³¿Ÿæñ - windows 1250
	if (chr >= 128)
		switch (chr)
		{
		case 0xea:
			chr = 'e';
			break;
		case 0xf3:
			chr = 'o';
			break;
		case 0xb9:
			chr = 'a';
			break;
		case 0x9c:
			chr = 's';
			break;
		case 0xb3:
			chr = 'l';
			break;
		case 0xbf:
			chr = 'z';
			break;
		case 0x9f:
			chr = 'z';
			break;
		case 0xe6:
			chr = 'c';
			break;
		case 0xf1:
			chr = 'n';
			break;
		}
	return chr;
}

unsigned char** get_lines(const char* file_name)
{
	if (!file_name)
		return NULL;

	unsigned char** lines = NULL;
	FILE* input_file = fopen(file_name, "r");

	if (input_file)
	{
		struct Line {
			unsigned char* str;
			struct Line* next;
		};
		struct Line head = {NULL, NULL};
		struct Line* iter = &head;
		int last_chr = '\n';
		int string_len;
		int buffer_len;
		int lines_counter = 0;
		Error error = NO_ERROR;

		while (last_chr != EOF)
		{
			int chr = fgetc(input_file);
			char DEBUG = chr;
			if (!is_whitespace(chr))
			{
				if (last_chr == '\n')
				{
					iter->next = malloc(sizeof(struct Line));
					if (!iter->next)
						error = ERROR_MALLOC_FAILURE;
					else
					{
						iter = iter->next;
						iter->next = NULL;
						buffer_len = BUFFER_SIZE;
						string_len = 0;
						lines_counter++;
						iter->str = malloc(BUFFER_SIZE);
						if (!iter->str)
							error = ERROR_MALLOC_FAILURE;
					}
				}
				error = put_to_buffer(iter->str, &buffer_len, to_lower_latin(chr), string_len++);
				last_chr = chr;
			}
			else
			{
				if (chr == EOF)
				{
					if (!is_whitespace(last_chr))
					{
						error = put_to_buffer(iter->str, &buffer_len, '\n', string_len++);
						error = put_to_buffer(iter->str, &buffer_len, '\0', string_len);
					}
					else if (last_chr != '\n')
					{
						error = put_to_buffer(iter->str, &buffer_len, '\n', string_len-1);
						error = put_to_buffer(iter->str, &buffer_len, '\0', string_len);
					}
					last_chr = chr;
				}
				else if (chr == '\n')
				{
					if (!is_whitespace(last_chr))
					{
						error = put_to_buffer(iter->str, &buffer_len, '\n', string_len++);
						error = put_to_buffer(iter->str, &buffer_len, '\0', string_len++);
					}
					else if (last_chr != '\n')
					{
						error = put_to_buffer(iter->str, &buffer_len, '\n', string_len-1);
						error = put_to_buffer(iter->str, &buffer_len, '\0', string_len++);
					}
					last_chr = chr;
				}
				else if (!is_whitespace(last_chr))
				{
					error = put_to_buffer(iter->str, &buffer_len, ' ', string_len++);
					last_chr = chr;
				}
			}
			
		}

		fclose(input_file);

		if (!error)
		{
			lines = malloc(sizeof(unsigned char*) * (lines_counter + 1));
			if (!lines)
			{
				error = ERROR_MALLOC_FAILURE;
				iter = head.next;
				while (iter)
				{
					free(iter->str);
					iter = iter->next;
				}
			}
			else
			{
				iter = head.next;
				for (int i = 0; iter; i++)
				{
					lines[i] = iter->str;
					iter = iter->next;
				}
				lines[lines_counter] = NULL;
			}
		}
		
		iter = head.next;
		while (iter)
		{
			struct Line* temp = iter;
			iter = iter->next;
			free(temp);
		}		
	}

	return lines;
}

unsigned char** find_header(unsigned char** list, const char* name)
{

	while (list && *list)
	{
		unsigned char buffer[BUFFER_SIZE + 1] = "";
		int read = sscanf(*list, "[%"TO_STRING(BUFFER_SIZE)"[^]]]\n", buffer);
		if (read == EOF)
			list = NULL;
		else
		{
			list++;
			if (read == 1 && !strcmp(name, buffer))
				break;
		}
	}

	return list;
}

Error get_settings(unsigned char** list, struct CPUSetup* setup)
{
	if (!list || !setup)
		return ERROR_NULL_POINTER;

	Error error = NO_ERROR;
	const char settings_str[] = "opcje";
	struct
	{
		const char* name;
		void* value_ptr;
	} setup_map[] = {
		{"polaczenie", &setup->as_link},
		{"inkrementacja", &setup->alu_inc},
		{"logiczne", &setup->alu_logic},
		{"arytmetyczne", &setup->alu_ext},
		{"stos", &setup->stack},
		{"rejestrx", &setup->reg_x},
		{"rejestry", &setup->reg_y},
		{"przerwania", &setup->intrp},
		{"wejscie", &setup->io},
		{"znaczniki", &setup->tags},
		{"adres", &setup->address_length},
		{"kod", &setup->code_length} };
	int map_size = sizeof(setup_map) / sizeof(setup_map[0]);
	
	list = find_header(list, settings_str);
	if (!list)
		error = ERROR_FILE_HANDLING;

	while (!error && *list)
	{
		unsigned char buffer[BUFFER_SIZE +1];
		unsigned int value;
		int read = sscanf(*list, "%"TO_STRING(BUFFER_SIZE)"[^=]=%u\n", buffer, &value);
		if (read == EOF)
			error = ERROR_FILE_HANDLING;
		else if (read != 2)
			break;
		else
		{
			bool found = false;
			for (int i = 0; i < map_size && !found; i++)
				if (!strcmp(buffer, setup_map[i].name))
				{
					*(unsigned char*)setup_map[i].value_ptr = value;
					found = true;
				}
			if (!found)
				error = ERROR_FILE_HANDLING;
			else
				list++;
		}	
	}

	return error;
}

char** get_instr_names(char** list, int* instr_num)
{
	if (!list || !instr_num)
		return NULL;

	Error error = NO_ERROR;
	const char instr_str[] = "rozkazy";
	const char number_str[] = "liczba";
	const char name_str[] = "nazwa";

	list = find_header(list, instr_str);
	if (!list)
		error = ERROR_FILE_HANDLING;

	if (!error)
	{
		unsigned char buffer[BUFFER_SIZE + 1] = "";
		int read = sscanf(*list, "%"TO_STRING(BUFFER_SIZE)"[^=]=%u\n", buffer, instr_num);
		if (read != 2 || strcmp(number_str, buffer))
			error = ERROR_FILE_HANDLING;
		else
			list++;
	}

	for (int line = 1; !error && *list && line <= *instr_num; line++, list++)
	{
		unsigned char name_buffer[BUFFER_SIZE + 1];
		unsigned int value;
		unsigned char instr_buffer[BUFFER_SIZE + 1];
		
		int read = sscanf(*list, 
			"%"TO_STRING(BUFFER_SIZE)"[^0-9]%u=%"TO_STRING(BUFFER_SIZE)"[^\n]\n", name_buffer, &value, instr_buffer);
		if (read != 3 || strcmp(name_str, name_buffer) || value != line)
			error = ERROR_FILE_HANDLING;
		else
			strcpy(*list, instr_buffer);
	}

	return error ? NULL : list - *instr_num;
}

struct RawInstruction {
	const char* name;
	struct RawLine
	{
		char** signals;
		int signals_num;
	} *lines;
	int lines_num;
};

Error get_instr(unsigned char** list, struct RawInstruction* r_instr)
{
	if (!list && !r_instr)
		return ERROR_NULL_POINTER;

	Error error = NO_ERROR;
	const char lines_str[] = "linie";
	const char line_str[] = "linia";
	const char instr_str[] = "rozkaz";
	const char arg_str[] = "argumenty";

	list = find_header(list, r_instr->name);
	if (!list)
		error = ERROR_FILE_HANDLING;

	int lines_num;
	if (!error)
	{
		unsigned char buffer[BUFFER_SIZE + 1] = "";
		int read = sscanf(*list, "%"TO_STRING(BUFFER_SIZE)"[^=]=%u\n", buffer, &lines_num);
		if (read != 2 || strcmp(lines_str, buffer))
			error = ERROR_FILE_HANDLING;
		else
			list++;
	}

	struct LinesList {
		struct InstrList {
			char* str;
			struct InstrList* next;
		} line;
		int instr_list_length;
		struct LinesList* next;
	};
	struct LinesList head = { NULL, NULL };
	struct LinesList* line_iter = &head;
	struct InstrList* instr_iter = &head.line;
	int lines_list_length = 0;
	char last_chr = ';';

	for (int line = 1; !error && *list && line <= lines_num; line++, list++)
	{
		unsigned char buffer[BUFFER_SIZE + 1];
		unsigned int value;

		int read = sscanf(*list, "%"TO_STRING(BUFFER_SIZE)"[^0-9]%u=", buffer, &value);
		if (read != 2 || strcmp(line_str, buffer) || value != line)
			error = ERROR_FILE_HANDLING;
		else
		{
			char* line = strchr(*list, '=') + 1;
			const char comment_str[] = "//";
			char* comment_pos = strstr(line, comment_str);
			if (comment_pos)
				*comment_pos = '\0';

			char chr;
			while (!error && (chr = *line))
			{
				if (chr == ' ' || chr == '\n' || chr == ';')
				{
					*line = '\0';
					if (last_chr != ';')
						last_chr = chr;
				}
				else
				{
					if (last_chr == ' ' || last_chr == '\n' || last_chr == ';')
					{
						if (last_chr == ';')
						{
							line_iter->next = (struct LinesList*)malloc(sizeof(struct LinesList));
							if (!line_iter->next)
								error = ERROR_MALLOC_FAILURE;
							else
							{
								line_iter = line_iter->next;
								line_iter->next = NULL;
								line_iter->line = (struct InstrList){ NULL, NULL };
								line_iter->instr_list_length = 0;
								instr_iter = &line_iter->line;
								lines_list_length++;
							}
						}

						if (!error && line_iter)
						{
							instr_iter->next = (struct InstrList*)malloc(sizeof(struct InstrList));
							if (!instr_iter->next)
								error = ERROR_MALLOC_FAILURE;
							else
							{
								instr_iter = instr_iter->next;
								instr_iter->next = NULL;
								instr_iter->str = line;
								line_iter->instr_list_length++;
							}
						}
					}
					last_chr = chr;
				}
				line++;
			}
		}
	}

	if (!error)
	{
		r_instr->lines_num = lines_list_length;
		r_instr->lines = malloc(sizeof(struct RawLine) * lines_list_length);
		if (!r_instr)
			error = ERROR_MALLOC_FAILURE;
		else
		{
			line_iter = head.next;
			for (int line = 0; line < lines_list_length && !error; line++)
			{
				r_instr->lines[line].signals_num = line_iter->instr_list_length;
				r_instr->lines[line].signals = malloc(sizeof(char*) * line_iter->instr_list_length);
				if (!r_instr)
					error = ERROR_MALLOC_FAILURE;
				else
				{
					struct InstrList* instr_iter = line_iter->line.next;
					for (int sig = 0; sig < line_iter->instr_list_length; sig++)
					{
						r_instr->lines[line].signals[sig] = instr_iter->str;
						instr_iter = instr_iter->next;
					}
				}
				line_iter = line_iter->next;
			}
		}

		line_iter = head.next;
		while (line_iter)
		{
			instr_iter = line_iter->line.next;
			while (instr_iter)
			{
				struct InstrList* temp = instr_iter;
				instr_iter = instr_iter->next;
				free(temp);
			}

			struct LinesList* temp = line_iter;
			line_iter = line_iter->next;
			free(temp);
		}
	}

	return error;
}

Error compile_instructions(const char* file_name, struct CPU* cpu)
{
	if (!file_name || !cpu)
		return ERROR_NULL_POINTER;

	Error error = NO_ERROR;
	unsigned char** instruction_list = get_lines(file_name);

	if (!instruction_list)
		error = ERROR_FILE_HANDLING;
	else
	{
		error = get_settings(instruction_list, cpu_get_setup(cpu));
		//if (cpu->setup.address_length > MAX_ADDRESS_LENGTH ||
		//	cpu->setup.code_length > MAX_CODE_LENGTH)
		//	error |= ERROR_FILE_HANDLING; // ???
	}

	unsigned char** names_list = NULL;
	int names_read = 0;
	struct RawInstruction* r_instr_array = NULL;
	if (!error)
	{
		names_list = get_instr_names(instruction_list, &names_read);
		if (!names_list)
			error = ERROR_FILE_HANDLING;
		else
		{
			r_instr_array = malloc(sizeof(struct RawInstruction) * names_read);
			if (!r_instr_array)
				error = ERROR_MALLOC_FAILURE;
		}	
	}

	for (int i = 0; i < names_read && !error; i++)
	{ 
		// Domyœlnie 1 argument
		if (names_list)
		{
			r_instr_array[i] = (struct RawInstruction){ names_list[i], NULL };
			error = get_instr(instruction_list, r_instr_array + i);

			// DEBUG
			if (!error) 
			{
				printf("rozkaz '%s'\n", r_instr_array[i].name);
				for (int line = 0; line < r_instr_array[i].lines_num; line++)
				{
					printf("linia %i: ", line);
					for (int sig = 0; sig < r_instr_array[i].lines[line].signals_num; sig++)
					{
						printf("'%s', ", r_instr_array[i].lines[line].signals[sig]);
					}
					printf("\n");
				}

			}

			if (!error)
			{
				struct {
					const char* instr_str;
					const char* arg_str;
					const char* if_str;
					const char* then_str;
					const char* else1_str;
					const char* else2_str;
					const char* end_str;
				} keywords = {
					.instr_str = "rozkaz",
					.arg_str = "argumenty",
					.if_str = "jezeli",
					.then_str = "to",
					.else1_str = "gdy",
					.else2_str = "nie",
					.end_str = "koniec"
				};

				struct Map
				{
					const char* name;
					void* value_ptr;
				};

				struct Map* signals_map = ;

			}
		}
	}



	return error;
}