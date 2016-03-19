/*
 *	libc/unistd/execve.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

static inline
__syscall3(int, exec,const char*, filename, char **, argv,char **, envp) ;

int execve(char *filename, char **argv, char **envp)
{
	return exec(filename, argv, envp);
}
