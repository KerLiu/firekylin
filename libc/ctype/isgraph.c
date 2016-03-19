/*
 *	libc/ctype/isgraph.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef isgraph

int isgraph(int c)
{
	return (__ctype[(c)+1]&(_P|_U|_L|_D));
}