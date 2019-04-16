
#include "all.h"

void printf_hex(char *buf, int len)
{
	int i;
	for(i = 0; i < len; i++)
	{
		printf("0x%x ", buf[i]);
	}
}


