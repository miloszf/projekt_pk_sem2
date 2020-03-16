#include <stdlib.h>

#include "error.h"

void print_error(Error *err)
{
	switch (*err)
	{
	case ERROR_VT_INIT_FAILED:
		printf("Blad incicjalizacji 'Virtual Terminal'.\n");
		break;
	case ERROR_GET_SCREEN_BUFFER_FAILED:
		printf("Blad podczas pobierania informacji z bufora ekranu konsoli.\n");
		break;
	case ERROR_CONSOLE_INPUT_READ_FAILED:
		printf("Blad podczas odczytywania informacji z bufora wejsciowego konsoli.\n");
		break;
	case ERROR_MEMORY_ALLOCATION_FAILED:
		printf("Blad podczas podczas proby alokowania pamieci.\n");
		break;
	case ERROR_NULL_POINTER_EXCEPTION:
		printf("Blad - wyluskanie wskaznika na NULL.\n");
		break;
	}
}