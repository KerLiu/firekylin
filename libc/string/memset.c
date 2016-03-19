/*
 *	libc/string/memset.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

void *memset(void *s, int c, size_t n)
{
	char *t = (char*) s;

	while (n--)
		*t++ = c;
	return s;
}
