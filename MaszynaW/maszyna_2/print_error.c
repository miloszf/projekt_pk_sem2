#include "print_error.h"

#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 50

void print_error(Error* error)
{
	if (!error || !(*error))
	{
		char stars[LINE_LENGTH + 1];
		memset(stars, '*', LINE_LENGTH);
		stars[LINE_LENGTH] = NULL;
		printf("%s\n", stars);
		if (error)
		{
			if (*error | ERROR_NULL_POINTER)
				printf("Wy³uskanie wska¿nika na NULL.\n");
		}
		else
			printf("\n");
		printf("%s\n", stars);
	}
}
