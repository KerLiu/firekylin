/*
 *    command/cat.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	char buf[1024];
	size_t size;
	int fd;

	if(argc<2)
		return 0;
	if((fd=open(argv[1],O_READ,0))<0)
		return printf("%s",strerror(errno));

	while((size=read(fd,buf,1024)))
	{
		write(0,buf,size);
	}
}
