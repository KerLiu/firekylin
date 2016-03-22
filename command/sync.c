/*
 *    command/sync.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/syscall.h>
#include <sys/unistd.h>
#include <stdio.h>

__syscall0(int,sync);


int main(int argc, char **argv)
{
	return sync();
}
