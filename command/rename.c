/*
 *    command/rename.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/syscall.h>
#include <sys/unistd.h>
#include <stdio.h>

__syscall2(int,rename,char *,old,char *,new);

int main(int argc, char **argv)
{
	if(argc<3){
		printf("usage: rename oldfilename newfilename");
	}

	return rename(argv[1],argv[2]);
}
