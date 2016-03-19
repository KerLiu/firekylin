/*
 *	libc/unistd/utime.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>
#include <sys/stat.h>

__syscall2(int, utime, const char*, filename, struct utimebuf*, utbuf);
