/*
 *	libc/string/strnlen.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

size_t strnlen(char *s, size_t n)
{
	char *t = s;

	while (n-- && *t)
		t++;
	return (t - s);
}
