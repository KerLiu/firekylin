/*
 *	libc/ctype/toupper.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef toupper

int toupper(int c)
{
	return islower(c) ? c-0x20 : c;
}
