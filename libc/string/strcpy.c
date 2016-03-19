/*
 *	libc/string/strcpy.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

char *strcpy(char *dst, char *src)
{
	char *tmp = dst;
	while (*src)
		*tmp++ = *src++;
	*tmp = 0;
	return dst;
}
