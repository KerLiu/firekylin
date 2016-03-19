/*
 *	libc/string/strcat.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

char *strcat(char *dst, char *src)
{
	char *tmp = dst;
	while (*tmp)
		tmp++;
	while (*src)
		*tmp++ = *src++;
	return dst;
}
