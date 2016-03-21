/*
 *	libc/time/time.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <time.h>

static inline __syscall1(int, getime, time_t*, time_loc);

time_t time(time_t *tloc)
{
	return getime(tloc);
}
