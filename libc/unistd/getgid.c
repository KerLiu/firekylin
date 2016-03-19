/*
 *	libc/unistd/getgid.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

__syscall0(gid_t, getgid);
