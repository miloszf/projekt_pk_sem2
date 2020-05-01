#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#pragma warning(disable : 4996)

#include "file_handling.h"

int main()
{
	char expected_tokens[][10] = {
		"czyt", "wys", "wei", "il"
	};
	
	printf("%c", (expected_tokens[0])[0]);
	(expected_tokens[0])[0] = 'z';
	printf("%c", (expected_tokens[0])[0]);
	return 0;
}