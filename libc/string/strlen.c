/*
 *	libc/string/strlen.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

size_t strlen(const char *s)
{
	char *p = (char*) s;
	while (*p)
		p++;
	return (size_t) (p - s);
}
