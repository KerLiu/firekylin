/*
 *	libc/string/strncpy.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

char *strncpy(char *dst, char *src, size_t n)
{
	int i = 0;
	char *t = dst;
	while (n-- && *src)
		*t++ = *src++;
	return dst;
}
