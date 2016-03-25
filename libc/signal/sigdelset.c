/*
 *	libc/time/sigdelset.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <signal.h>
#include <errno.h>

int sigdelset(sigset_t *set, int signo)
{
	if (signo < 0 || signo > 32) {
		errno = EINVAL;
		return -1;
	}

	*set &= ~(1 << (signo - 1));
	return 0;
}
