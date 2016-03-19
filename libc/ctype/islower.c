/*
 *	libc/ctype/islower.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef islower

int islower(int c)
{
	return (__ctype[(c)+1]&(_L));
}