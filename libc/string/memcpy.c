/*
 *	libc/string/memcpy.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

void * memcpy(void *dst, void *src, size_t n)
{
	char *d = (char*) dst;
	char *s = (char*) src;

	while (n--)
		*d++ = *s++;

	return dst;
}
