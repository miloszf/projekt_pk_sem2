#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <wchar.h>

#define X 128
#define Y 64

int main()
{
	HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (out_handle == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	CHAR_INFO *char_info = malloc(sizeof(CHAR_INFO)*X*Y);
	if (!char_info)
		return -1;
	COORD size = { X, Y };
	COORD coord = { 0, 0};
	SMALL_RECT rect= {0, 0, X-1, Y-1};

	for (unsigned i = 0; i < X*Y; i++)
	{
		char_info[i].Char.UnicodeChar = '0';
		char_info[i].Attributes = i % 256;
		//DWORD d;
		
		
		//printf("%u - '%cl'\t", j, i);
		//if (j++ && !(j % 8))
		//	printf("\n");
		
	}

	if (!WriteConsoleOutput(out_handle, char_info, size, coord, &rect))
		return -1;

	//for (unsigned char c = 1; c <= 50; c++)
	//	printf("%u\n", c);
	//	wprintf("%u - ╗f\n", c);
	_getch();
	return 0;
}