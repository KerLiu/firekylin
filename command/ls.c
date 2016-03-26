/*
 *    command/ls.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char **argv)
{
	struct dirent *de;
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

		entries = size / sizeof(struct dirent);
		de = (struct dirent *) buf;

		while (entries--) {
			if (de->d_ino)
				printf("%s\t", de->d_name);
			de++;
		}
	}
	printf("\n");
}
