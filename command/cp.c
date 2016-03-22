/*
 *    command/cp.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int fd1,fd2,size;
	char buf[1024];

	if(argc<3){
		printf("%s\n","usage:cp src dst");
		_exit(0);
	}

	fd1=open(argv[1],O_READ,0);
	if(fd1<0){
		printf("%s,%s\n",argv[1],strerror(errno));
	}

	fd2=open(argv[2],O_WRITE,0);
	if(fd2<0){
		printf("%s,%s\n",argv[2],strerror(errno));
	}

	while((size=read(fd1,buf,1024))){
		write(fd2,buf,size);
	}
}
