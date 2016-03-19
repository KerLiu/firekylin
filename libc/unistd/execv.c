/*
 *	libc/unistd/execv.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

static inline
__syscall3(int, exec, char*, filename, char **, argv, char **, envp);

int execv(const char * pathname, char ** argv)
{
	return exec(pathname,argv,environ);
}
