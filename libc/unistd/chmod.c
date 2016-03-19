/*
 *	libc/unistd/chmod.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

__syscall2(int, chmod, const char*, filename, mode_t, mode);
