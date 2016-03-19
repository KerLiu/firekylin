/*
 *	libc/string/strcmp.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

int strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1==*s2){
		s1++;
		s2++;
	}
		
	return (int)(*s1-*s2);
}