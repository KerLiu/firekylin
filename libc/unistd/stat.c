/*
 *	libc/unistd/stat.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

__syscall2(int, stat, char*, filename, struct stat*, statbuf);
