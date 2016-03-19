/*
 *	libc/ctype/isspace.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef isspace

int isspace(int c)
{
	return (__ctype[(c)+1]&(_S));
}
