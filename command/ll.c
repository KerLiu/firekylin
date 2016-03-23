/*
 *    command/ll.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#define NAME_LEN	14
struct dir_entry {
	unsigned short ino;
	char name[NAME_LEN];
};

int main(int argc, char **argv)
{
	struct stat statbuf;
	struct dir_entry *de;
	char buf[1024];
	int fd;
	int size = 0;
	int entries;

	if (argc > 1) {
		fd = open(argv[1], O_READ, 0);
	} else {
		fd = open(".", O_READ, 0);
	}
	if (fd < 0)
		printf("%s\n", strerror(errno));

	while ((size = read(fd, buf, 1024))) {

		entries = size / sizeof(struct dir_entry);
		de = (struct dir_entry *) buf;

		while (entries--) {
			if (!de->ino){
				de++;
				continue;
			}
			stat(de->name,&statbuf);
			printf("%s\t", de->name);
			switch(statbuf.st_mode & S_IFMT){
				case S_IFREG :
					printf("%s\t","FILE");
					break;
				case S_IFDIR:
					printf("%s\t","DIR");
					break;
				case S_IFBLK:
					printf("%s\t","BLK");
					break;
				case S_IFCHR:
					printf("%s\t","CHAR");
					break;
			}
			printf("%d\n",statbuf.st_size);
			de++;
		}
	}
	printf("\n");
}
