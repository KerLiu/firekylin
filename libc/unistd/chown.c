/*
 *	libc/unistd/chown.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

__syscall3(int, chown, const char*, filename, uid_t, uid, gid_t, gid);
