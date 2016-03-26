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
#include <dirent.h>

int main(int argc, char **argv)
{
	struct stat statbuf;
	DIR *dp;
	struct dirent *de;

	if (argc > 1) {
		dp = opendir(argv[1]);
	} else {
		dp = opendir(".");
	}

	while ((de = readdir(dp))) {
		stat(de->d_name, &statbuf);
		printf("%s\t", de->d_name);
		switch (statbuf.st_mode & S_IFMT) {
			case S_IFREG:
				printf("%s\t", "FILE");
				break;
			case S_IFDIR:
				printf("%s\t", "DIR");
				break;
			case S_IFBLK:
				printf("%s\t", "BLK");
				break;
			case S_IFCHR:
				printf("%s\t", "CHAR");
				break;
		}
		printf("%d\t", statbuf.st_nlink);
		printf("%d\n", statbuf.st_size);
	}
}
