/*
 *	libc/time/sigismember.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <signal.h>
#include <errno.h>

int sigismember(sigset_t *set, int signo)
{
	if (signo < 0 || signo > 32) {
		errno = EINVAL;
		return -1;
	}

	return (*set) >> (signo - 1) & 1;
}
