/*
 *	libc/signal/sigprocmask.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <signal.h>

static inline
__syscall3(int, sigmask, int,how,sigset_t *,set,sigset_t *,newset);

int sigprocmask(int how, sigset_t *set, sigset_t *oldset)
{
	return sigmask(how, set, oldset);
}
