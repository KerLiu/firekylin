/*
 *    command/ed.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/syscall.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

char buf[100];
int get_line()
{
	int size;
	size=read(0,buf,100);
	return size;
}

int main(int argc, char **argv)
{
	int fd;
	int size;
	if(argc<2){
		printf("Usage:ed name ...");
	}
	if(create(argv[1],S_IFREG |0777,0)<0){
		printf("%s",strerror(errno));
		_exit(0);
	}
	fd=open(argv[1],O_RDWR,0);
	if(fd<0){
		printf("%s",strerror(errno));
		_exit(0);
	}
	while((size=get_line())){
		write(fd,buf,size);
	}
	close(fd);
}
