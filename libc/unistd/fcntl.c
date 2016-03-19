/*
 *	libc/unistd/fcntl.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

__syscall3(int, fcntl, int, fd, int, cmd, long ,arg);
