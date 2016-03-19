/*
 *	libc/string/strncat.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

char *strcat(char *dst, char *src ,size_t n)
{
	char *tmp = dst;

	while (*tmp)
		tmp++;

	while (n-- && *src)
		*tmp++ = *src++;

	return dst;
}