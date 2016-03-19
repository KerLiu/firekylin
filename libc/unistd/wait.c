/*
 *	libc/unistd/wait.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

static inline
__syscall3(int, pwait, pid_t, pid, long *, status, int, options);

int wait(long *status)
{
	return pwait(0,status,0);
}
