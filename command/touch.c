/*
 *    command/touch.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/syscall.h>
#include <sys/unistd.h>
#include <stdio.h>

/*
 * create a file
 */

__syscall1(int,fcreate,char *,filename);

int main(int argc, char **argv)
{
	if(argc<2){
		printf("%s","touch filename");
	}

	fcreate(argv[1]);
}
