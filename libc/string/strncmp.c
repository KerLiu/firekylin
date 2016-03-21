/*
 *	libc/string/strncmp.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

int strncmp(char *s1, char *s2, size_t n)
{
	while (n--) {
		if (*s1 && (*s1 == *s2)) {
			s1++;
			s2++;
		} else {
			return (*s1 - *s2);
		}
	}
	return 0;
}
