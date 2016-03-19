/*
 *	libc/ctype/isalnum.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef isalnum

int isalnum(int c)
{
	return (__ctype[(c)+1]&(_U|_L|_D));
}
