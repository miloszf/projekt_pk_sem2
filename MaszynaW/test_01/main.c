#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#pragma warning(disable : 4996)

#include "file_handling.h"

int main()
{
	wchar_t w_str[20];
	char str[] = "weja";
	int a = 101;
	int chars_read = swprintf(w_str, (sizeof(w_str) / sizeof(*w_str)), L"%s:%u", str, a);
	wprintf(L"%ls", w_str);
	
	return 0;
}