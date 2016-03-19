/*
 *	libc/stdlib/abs.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

int abs(int i)
{
	return i >= 0 ? i : -i;
}
