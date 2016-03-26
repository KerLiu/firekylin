/*
 *	libc/unistd/create.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>
#include <sys/stat.h>

__syscall3(int, create, char *,filename, mode_t, mode, long, arg);

