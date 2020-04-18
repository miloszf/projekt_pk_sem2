//#include <stdlib.h>
//#include <stdio.h>
//#include <ctype.h>
//#include <stdbool.h>
//#include <string.h>
//#pragma warning(disable : 4996)
//
//#include "instruction.h"
//
//#define STR(x) #x
//#define TO_STRING(x) STR(x)
//
//#define BUFFER_SIZE 255
//#define INSTRUCTIONS_STR "rozkazy"
//
//#define WARNING_EOF ERROR_RESERVED 
//
//struct InstructionsNames
//{
//	unsigned int number;
//	char** names;
//};
//
//struct InstructionsSignals
//{
//	struct RawSignal
//	{
//		char* name;
//		char** lines;
//		unsigned int lines_num;
//	};
//	struct RawSignal* signals;
//	unsigned int signals_num;
//};
//
//void to_lower(char* str)
//{
//	if (str)
//		while (*str)
//		{
//			*str = tolower(*str);
//			str++;
//		}
//}
//
//Error instructions_get_setup(const char* file_name, struct CPUSetup* setup)
//{
//	if (!file_name || !setup)
//		return ERROR_NULL_POINTER;
//
//	FILE* input_file = fopen(file_name, "r");
//	if (!input_file)
//		return ERROR_FILE_HANDLING;
//
//	Error error = NO_ERROR;
//	const char settings_str[] = "[opcje]";
//	char buffer[BUFFER_SIZE + 1];
//	int strings_read;
//
//	do
//	{
//		strings_read = fscanf(input_file, "%"TO_STRING(BUFFER_SIZE)"s\n", buffer);
//
//		if (strings_read == EOF || !strings_read)
//			error = ERROR_INSTRUCTIONS_FILE;
//		else
//			to_lower(buffer);
//
//	} while (!error && strcmp(buffer, settings_str));
//
//	if (!error)
//	{
//		struct
//		{
//			const char* name;
//			void* value_ptr;
//		} setup_map[] = {
//			{"po""\xb3\xb9""czenie", &setup->as_link},
//			{"polaczenie", &setup->as_link},
//			{"inkrementacja", &setup->alu_inc},
//			{"logiczne", &setup->alu_logic},
//			{"arytmetyczne", &setup->alu_ext},
//			{"stos", &setup->stack},
//			{"rejestrx", &setup->reg_x},
//			{"rejestry", &setup->reg_y},
//			{"przerwania", &setup->intrp},
//			{"wej""\x9c""cie", &setup->io},
//			{"wejscie", &setup->io},
//			{"znaczniki", &setup->tags},
//			{"adres", &setup->address_length},
//			{"kod", &setup->code_length} };
//
//		size_t map_size = sizeof(setup_map) / sizeof(setup_map[0]);
//		bool found;
//		bool stop = false;
//
//		do
//		{
//			found = false;
//			unsigned int value;
//			int strings_read = fscanf(input_file,
//				"%"TO_STRING(BUFFER_SIZE)"[^\n=]=%u\n",
//				buffer, &value);
//
//			if (strings_read == EOF || strings_read == 1)
//				stop = true;
//			else if (!strings_read)
//			{
//				error = ERROR_INSTRUCTIONS_FILE;
//				stop = true;
//			}
//
//			if (!stop)
//			{
//				to_lower(buffer);
//				for (size_t i = 0; i < map_size && !found; i++)
//					if (!strcmp(buffer, setup_map[i].name))
//					{
//						*(unsigned char*)setup_map[i].value_ptr = value;
//						found = true;
//					}
//			}
//		} while (found && !stop);
//	}
//
//	fclose(input_file);
//	return error;
//}
//
//Error instructions_get_names(const char* file_name, struct InstructionsNames** instr_names)
//{
//	if (!file_name || !instr_names)
//		return ERROR_NULL_POINTER;
//
//	FILE* input_file = fopen(file_name, "r");
//	if (!input_file)
//		return ERROR_FILE_HANDLING;
//
//	struct InstructionsNames* i_names = malloc(sizeof(struct InstructionsNames));
//	if (!i_names)
//		return ERROR_MALLOC_FAILURE;
//
//	*instr_names = i_names;
//	Error error = NO_ERROR;
//	const char instructions_str[] = "[rozkazy]";
//	char buffer[BUFFER_SIZE + 1];
//	int strings_read;
//
//	do
//	{
//		strings_read = fscanf(input_file, "%"TO_STRING(BUFFER_SIZE)"s\n", buffer);
//
//		if (strings_read == EOF || !strings_read)
//			error = ERROR_INSTRUCTIONS_FILE;
//		else
//			to_lower(buffer);
//
//	} while (!error && strcmp(buffer, instructions_str));
//
//	strings_read = fscanf(input_file,
//		"%"TO_STRING(BUFFER_SIZE)"[^=]=%u\n",
//		buffer, &i_names->number);
//
//	const char number_str[] = "liczba";
//
//	if (strings_read == EOF || strings_read < 2)
//		error = ERROR_INSTRUCTIONS_FILE;
//	else
//	{
//		to_lower(buffer);
//		if (strcmp(buffer, number_str))
//			error = ERROR_INSTRUCTIONS_FILE;
//	}
//		
//	if (!error && i_names->number)
//	{
//		const char name_str[] = "nazwa";
//		i_names->names = malloc(sizeof(char*) * i_names->number);
//
//		if (!i_names->names)
//			error = ERROR_MALLOC_FAILURE;
//		else
//			for (unsigned int i = 0; i < i_names->number; i++)
//				i_names->names[i] = NULL;
//
//		for (unsigned int i = 0; i < i_names->number && !error; i++)
//		{
//			unsigned int line;
//			char name_buffer[BUFFER_SIZE + 1] = "";
//
//			strings_read = fscanf(input_file,
//				"%"TO_STRING(BUFFER_SIZE)"[A-Za-z]%u=%"TO_STRING(BUFFER_SIZE)"s\n",
//				buffer, &line, name_buffer);
//			line--;
//			to_lower(buffer);
//			if (strings_read == EOF || strings_read < 3 || 
//				line >= i_names->number || i_names->names[line] ||
//				strcmp(buffer, name_str))
//				error = ERROR_INSTRUCTIONS_FILE;
//			else
//			{
//				size_t name_len = strlen(name_buffer) + 1;
//				char* new_name = malloc(name_len);
//				if (!new_name)
//					error = ERROR_MALLOC_FAILURE;
//				else
//				{
//					memcpy(new_name, name_buffer, name_len);
//					i_names->names[line] = new_name;
//				}
//			}
//		}
//	}
//
//	fclose(input_file);
//	// DEBUG
//	if (!error)
//		for (unsigned int i = 0; i < i_names->number; i++)
//			printf("%s\n", i_names->names[i]);
//	return error;
//}
//
//
//
////Error instructions_get_signals(const char* file_name, struct InstructionsNames** instr_names, struct InstructionsSignals** instr_signals)
////{
////	if (!file_name || !instr_names || instr_signals)
////		return ERROR_NULL_POINTER;
////
////	if (!(*instr_names)->number)
////		return NO_ERROR;
////
////	FILE* input_file = fopen(file_name, "r");
////	if (!input_file)
////		return ERROR_FILE_HANDLING;
////
////	Error error = NO_ERROR;
////	unsigned int signals_read = 0;
////	struct InstructionsNames* i_names = *instr_names;
////	struct InstructionsSignals* i_signals = malloc(sizeof(struct InstructionsSignals));
////	if (!i_signals)
////		error =  ERROR_MALLOC_FAILURE;
////	else
////		i_signals->
////	
////	while (!error && signals_read < i_names->number)
////	{
////		char line_buffer[BUFFER_SIZE + 1] = "";
////		char name_buffer[BUFFER_SIZE + 1] = "";
////		unsigned int strings_read = 0;
////		unsigned int i_index = 0;
////		do
////		{
////			strings_read = fscanf(input_file, "%"TO_STRING(BUFFER_SIZE)"s\n", line_buffer);
////
////			if (strings_read == EOF || !strings_read)
////				error = ERROR_INSTRUCTIONS_FILE;
////			else
////			{
////				to_lower(line_buffer);
////				strings_read = sscanf(line_buffer, "[%"TO_STRING(BUFFER_SIZE)"[^]]]", name_buffer);
////
////				if (strings_read != EOF && strings_read)
////					for (unsigned int i = 0; i < i_names->number && !i_index; i++)
////						if (!strcmp(name_buffer, i_names->names[i]))
////							i_index = i;
////			}
////		} while (!error && !i_index);
////
////		if (i_index)
////		{
////			const char lines_str[] = "linie";
////
////		}
////
////		strings_read = fscanf(input_file,
////			"%"TO_STRING(BUFFER_SIZE)"[^=]=%u\n",
////			line_buffer, &i_names->number);
////	} 
////	
////}
//
//Error instructions_compile(struct InstructionsNames** names, struct InstructionList** list)
//{
//	return NO_ERROR;
//}
///*
//Error instructions_init(const char* file_name, struct CPUSetup* setup, struct RawInstructionList** raw)
//{
//	if (!file_name || !setup || !raw)
//		return ERROR_NULL_POINTER;
//
//	Error error = NO_ERROR;
//
//	FILE* input_file = fopen(file_name, "r");
//
//	if (!input_file)
//		return ERROR_FILE_HANDLING;
//
//	char line_buffer[BUFFER_SIZE + 1] = "";
//	int strings_read = fscanf(input_file, "%"TO_STRING(BUFFER_SIZE)"s\n", line_buffer);
//
//	if (strings_read == EOF || !strings_read)
//		error = ERROR_INSTRUCTIONS_FILE;
//
//	//strings_read = fscanf(input_file, "[%"TO_STRING(BUFFER_SIZE)"[^]]]\n", line_buffer);
//
//	while (!error)
//	{
//		char scan_buffer[BUFFER_SIZE + 1];
//		strings_read = sscanf(line_buffer, "[%"TO_STRING(BUFFER_SIZE)"[^]]]", scan_buffer);
//
//		if (strings_read == EOF)
//			return NO_ERROR;
//		else if (!strings_read)
//			return ERROR_INSTRUCTIONS_FILE;
//
//		to_lower(scan_buffer);
//
//		if (!strcmp(scan_buffer, SETTINGS_STR))
//			error = load_settings(input_file, setup, line_buffer);
//		else if (!strcmp(scan_buffer, INSTRUCTIONS_STR))
//			error = load_instructions_names(input_file, raw, line_buffer);
//		//else
//			//error = load_instructions(input_file, raw, line_buffer);
//	}
//
//	return error & (~WARNING_EOF);
//}
//*/