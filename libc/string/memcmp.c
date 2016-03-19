/*
 *	libc/string/memcmp.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

int memcmp(const void *buf1, const void *buf2, size_t n)
{
	char *s1 = (char *) buf1;
	char *s2 = (char *) buf2;

	while (n-- && *s1 == *s2) {
		s1++;
		s2++;
	}

	return (*s1 - *s2);
}
