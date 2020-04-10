#include <stdio.h>
#include <assert.h>

void fun_a(struct T* ptr);
void fun_b(struct T* ptr);

struct T {
	struct T* next;
	void (*fun)(struct T*);
};

void fun_a(struct T* ptr)
{
	assert(ptr);
	ptr->next = ptr;
	ptr->fun = &fun_b;
	printf("fun_a\n");
}

void fun_b(struct T* ptr)
{
	assert(ptr);
	ptr->next = NULL;
	ptr->fun = &fun_a;
	printf("fun_b\n");
}

#include <stdint.h>

#define MAX_ADDR_BITS 16
#define MAX_CODE_BITS 8
#define U -1L

typedef int64_t var;
typedef uint64_t u_var;

#define WORD_LENGTH (MAX_ADDR_BITS + MAX_CODE_BITS)
#define WORD_MASK ((u_var)(-1) >> (sizeof(var) * 8 - WORD_LENGTH))

int main()
{
	u_var a = (var)-127;
	u_var b = WORD_MASK;
	printf("b = %08X\n", b);

	u_var c = U;
	printf("c = %08Xl\n", c);

	for (int i = 0; i < 10; i++)
	{
		printf("a = %08Xl\n", a);
		a = a >> 1;
	}
}