#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#pragma warning(disable : 4996)

#include "file_handling.h"
#include "error.h"

#define BUFFER_SIZE 256

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

char** get_lines(const char* file_name)
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

Error compile_instruction_list(const char* file_name)
{
	if (!file_name)
		return ERROR_NULL_POINTER;

	Error error = NO_ERROR;
	unsigned char** list = get_lines("MaszynaW.lst");
	if (list)
	{
		unsigned char** line = list;
	}
	else
		error = ERROR_FILE_HANDLING;
}
