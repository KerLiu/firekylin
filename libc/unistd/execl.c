/*
 *	libc/unistd/execl.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

static inline
__syscall3(int, exec, char*, filename, char **, argv, char **, envp);

int execl(const char * pathname, char * arg0, ...)
{
	return exec(pathname,&arg0,environ);
}
