/*
 *	libc/unistd/setuid.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

__syscall3(int, open, char*, filename, int, flag, long, arg);
