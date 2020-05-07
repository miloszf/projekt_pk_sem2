#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#pragma warning(disable : 4996)

#include "file_handling.h"

int main()
{
	char str[] = "Linia-125=asd\n";
	char buff[20];
	int unused;
	int read = sscanf_s(str, "Linia%d=%[^\n]\n", &unused, buff, 20);
	char buffer[8][20];
	return 0;
}