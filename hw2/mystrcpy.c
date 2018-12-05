#include <string.h>
#include <stdio.h>

void mystrcpy(char *dst, char *src) 
{
	while (*src) {
		printf(src);
		*dst++ = *src++;
	}

	*dst = *src;
}
