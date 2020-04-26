#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "file.h"
#include "map.h"
#include "error.h"
#include "vector.h"

#define STR(x) #x
#define TO_STRING(x) STR(x)
#define BUFFER_SIZE 256

struct FileHandler
{
	struct Vector* instructions_lines_vect;
};

struct FileHandler* file_handler_init()
{
	struct FileHandler* new_handler = malloc_s(sizeof(struct FileHandler));
	*new_handler = (struct FileHandler){ NULL };
	//new_handler->instructions_lines_vect = vector_init(sizeof(unsigned char*));
	return new_handler;
}

void file_handler_delete(struct FileHandler* handler)
{
	if (handler)
	{
		if (handler->instructions_lines_vect)
			vector_delete(handler->instructions_lines_vect);
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
		else
		{
			line_index++;
			if (read == 1 && !strcmp(name, buffer))
				break;
		}
	}

	return line_index;
}

bool file_import_setup(const char* file_name, struct FileHandler* files_handler, struct Map* pref_map)
{
	check_for_NULL(file_name);
	check_for_NULL(files_handler);
	check_for_NULL(pref_map);

	if (files_handler->instructions_lines_vect)
	{
		vector_delete(files_handler->instructions_lines_vect);
		files_handler->instructions_lines_vect = NULL;
	}

	struct Vector* lines_vect = get_lines(file_name);
	Error error = error_get();

	if (!error)
	{
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

		if (!error)
			files_handler->instructions_lines_vect = lines_vect;
	}
	else
		vector_delete(lines_vect);

	if (error)
		error_set(error);

	return !error;
}