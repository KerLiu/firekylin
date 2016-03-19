/*
 *	libc/unistd/execle.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

static inline
__syscall3(int, exec, char*, filename, char **, argv, char **, envp);

int execle(const char * pathname, char * arg0, ...)
{
	char ** env = &arg0;

	while (!*env)
		env++;

	return exec(pathname,&arg0,env);
}
