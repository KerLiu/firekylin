/*
 *	libc/string/memchr.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

void * memchr(const void * buf, int chr, size_t n)
{
	char *s = (char *) buf;

	while (n-- && *s != chr) {
		s++;
	}

	return n ? s : NULL;
}
