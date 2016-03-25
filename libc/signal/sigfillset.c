/*
 *	libc/time/sigfillset.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <signal.h>
#include <errno.h>

int sigefillset(sigset_t *set)
{
	*set = ~(sigset_t) 0;
	return 0;
}
