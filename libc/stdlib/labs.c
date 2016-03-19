/*
 *	libc/stdlib/labs.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

long labs(long i)
{
	return i >= 0 ? i : -i;
}
