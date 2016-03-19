/*
 *	libc/unistd/setuid.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

__syscall1(uid_t,setuid,uid_t,uid);
