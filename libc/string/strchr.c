/*
 *	libc/string/strchr.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

char * strchr(const char * str, char ch)
{
	char *s = (char *) str;
	
	while (*s && *s!= ch)
		s++;

	return  *s ? s : NULL;
}