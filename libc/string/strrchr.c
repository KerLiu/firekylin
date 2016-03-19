/*
 *	libc/string/strrchr.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

char * strrchr(const char * str, int ch)
{
	char *s = (char *) str;
	while (*str)
		str++;
	while (str-- != s && *str != (char) ch)
		;
	return (s <= str ? NULL : s);
}
