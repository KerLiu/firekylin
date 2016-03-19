/*
 *	libc/unistd/_exit.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/unistd.h>

void _exit(int status)
{
	asm("int $0x30" : :"a"(__NR_exit),"b"(status));
}
