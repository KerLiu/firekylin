/*
 *	libc/unistd/write.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

__syscall3(ssize_t, write, int, fd, char*, buf, size_t, size);
