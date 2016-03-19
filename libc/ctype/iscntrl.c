/*
 *	libc/ctype/iscntrl.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <ctype.h>

#undef iscntrl

int iscntrl(int c)
{
	return (__ctype[(c)+1]&(_C));
}