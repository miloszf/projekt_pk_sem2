//#include <stdlib.h>
//#include <string.h>
#include <stdio.h>
//#include <locale.h>
//#include <wchar.h>
//#pragma warning(disable : 4996)

#include "error.h"

//#define LOG (struct CrashLog) { __FILE__, __func__, __LINE__ }

int main()
{
	printf("przed\n");
	init_crash_log();
	void* ptr = NULL;
	//CHECK_IF_NULL(ptr);
	//if (error())
	CRASH_LOG(LOG_UNKNOWN_VALUE);
	//_crash_log(LOG, NULL_DEREFERENCE);
	printf("po\n");
	return 0;
}