/*
 *	libc/time/sigemptyset.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <signal.h>
#include <errno.h>

int sigemptyset(sigset_t *set)
{
	*set = 0;
	return 0;
}
