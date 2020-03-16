//#include <stdlib.h>
#include <stdio.h>

#include "graphics.h"

#define ESC "\x1b"
#define CSI "\x1b["

void draw(int x, int y)
{
    for (int row = 0; row < y; row++)
    {
        printf("<- linia %2i ", row);
        for (int ch = 0; ch < x - 13; ch++)
            printf("-");
        printf(">");
        if (row != y - 1)
            printf("\n");
    }
    printf(CSI "0;0H");
}