/*
 *	libc/ctype/isxdigit.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef isxdigit

int isxdigit(int c)
{
	return (__ctype[(c)+1]&(_D|_X));
}
