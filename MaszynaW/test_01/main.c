#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#pragma warning(disable : 4996)

#include "file_handling.h"

int main()
{
	//setlocale(LC_ALL, "Polish_Poland.1250");
	//unsigned char** list = get_lines("MaszynaW.lst");
	//if (list)
	//{
	//	unsigned char** line = list;
	//	while (*line)
	//	{
	//		printf("%s", *line);
	//		line++;
	//	}
	//}
	Error error = compile_instruction_list("MaszynaW.lst");
	
	return 0;
}