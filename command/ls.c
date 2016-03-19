/*
 *    command/ls.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

#define NAME_LEN	14
struct dir_entry {
	unsigned short ino;
	char name[NAME_LEN];
};

int main(void)
{
	char buf[41];
	int size;
	struct stat statbuf;
	int fd;
	int fd2;

	stat("/sys/dev/tty1", &statbuf);

	printf("dev %x\n", statbuf.st_dev);
	printf("ino %x\n", statbuf.st_ino);
	printf("mode %x\n", statbuf.st_mode);
	printf("link %x\n", statbuf.st_nlink);
	printf("rdev %x\n",statbuf.st_rdev);
	printf("uid %x\n", statbuf.st_uid);
	printf("gid %x\n", statbuf.st_gid);
	printf("size %d\n", statbuf.st_size);

	fd2=dup(0);
	fstat(fd2, &statbuf);

	printf("dev %x\n", statbuf.st_dev);
	printf("ino %x\n", statbuf.st_ino);
	printf("mode %x\n", statbuf.st_mode);
	printf("link %x\n", statbuf.st_nlink);
	printf("rdev %x\n",statbuf.st_rdev);
	printf("uid %x\n", statbuf.st_uid);
	printf("gid %x\n", statbuf.st_gid);
	printf("size %d\n", statbuf.st_size);
}
