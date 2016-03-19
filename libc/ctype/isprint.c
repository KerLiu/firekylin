/*
 *	libc/ctype/isprint.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef isprint

int isprint(int c)
{
	return (__ctype[(c)+1]&(_P|_U|_L|_D));
}