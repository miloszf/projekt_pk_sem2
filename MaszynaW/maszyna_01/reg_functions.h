#ifndef REG_FUNCTIONS_H
#define REG_FUNCTIONS_H

#include "settings.h"

/////////////////// UWAGA NA KOLEJNOŒÆ!!!!
/////////////////// a - wartoœæ do przetworzenia
/////////////////// b - wartoœæ z rejestru



// a >> 1
var shift_right(var a, var b);
// a / b
var divide(var a, var b);
// a * b
var multiply(var a, var b);
// a
var return_first(var a, var b);
// a - b
var subtract(var a, var b);
// a + b
var add(var a, var b);
// a + 1
var increment(var a, var b);
// a - 1
var decrement(var a, var b);
// ~a
var bitwise_complement(var a, var b);
// a | b
var bitwise_or(var a, var b);
// a & b
var bitwise_and(var a, var b);

#endif
