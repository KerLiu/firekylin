/*
 *	libc/unistd/mkdir.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>
#include <sys/stat.h>

static inline
__syscall3(int, create, char *,filename, mode_t, mode, long, arg);

int mkdir(char *filename,mode_t mode)
{
	return create(filename,S_IFDIR|(mode &0777),0);
}
