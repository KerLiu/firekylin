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
	struct dir_entry *de;
	char buf[1024];
	int fd;
	int size=0;
	int entries;
	fd=open(".",O_READ,0);

	size=read(fd,buf,1024);

		entries=size/sizeof(struct dir_entry);
		de=(struct dir_entry *)buf;

		while(entries--){
			printf("%s\t",de->name);
			de++;
		}
		printf("\n");
}
