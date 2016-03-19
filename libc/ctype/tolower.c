/*
 *	libc/ctype/tolower.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef tolower

int tolower(int c)
{
	if(isupper(c))
		c+=0x20;
	
	return c;
}