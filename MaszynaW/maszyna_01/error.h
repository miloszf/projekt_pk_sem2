#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

typedef uint16_t Error;

#define NO_ERROR				0x0000
#define ERROR_NULL_POINTER		0x0001
#define ERROR_MALLOC_FAILURE	0x0002
#define ERROR_FILE_HANDLING		0x0004
#define ERROR_INSTRUCTIONS_FILE	0x0008


//#define WARNING_NO_INPUT		0x8000
#define ERROR_RESERVED			0x8000

#endif