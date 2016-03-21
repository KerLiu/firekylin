/*
 *	libc/ctype/tolower.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef tolower

int tolower(int c)
{
	return isupper(c) ? c+0x20 : c;
}
