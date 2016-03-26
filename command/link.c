/*
 *    command/link.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/syscall.h>
#include <sys/unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	if(argc<3){
		printf("Usage:link name newname");
	}
	return link(argv[1],argv[2]);
}
