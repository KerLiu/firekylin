/*
 *	libc/ctype/isalpha.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef isalpha

int isalpha(int c)
{
	return (__ctype[(c)+1]&(_U|_L));
}
