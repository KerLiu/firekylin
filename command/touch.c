/*
 *    command/touch.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/syscall.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <stdio.h>

/*
 * create a file
 */

int main(int argc, char **argv)
{
	if(argc<2){
		printf("%s","touch filename");
	}
	create(argv[1],S_IFREG|0777,0);
}
