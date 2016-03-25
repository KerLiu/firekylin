/*
 *	libc/signal/sigaction.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>

static inline
__syscall3(int, sigact,int,signr, struct sigaction *,newact,
		struct sigaction *,oldact);

int sigaction(int signr, struct sigaction *newact, struct sigaction *oldact)
{
	return sigact(signr, newact, oldact);
}
