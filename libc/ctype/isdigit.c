/*
 *	libc/ctype/isdigit.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef isdigit

int isdigit(int c)
{
	return (__ctype[(c)+1]&(_D));
}