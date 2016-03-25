/*
 *	libc/dirent/opendir.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

DIR * opendir(char * dirname)
{
	int fd;
	struct stat statbuf;
	DIR * dp;

	if ((fd = open(dirname, O_READ)) < 0)
		return NULL;
	if (fstat(fd, &statbuf) < 0 ||
	    !S_ISDIR(statbuf.st_mode) ||
	    !(dp =malloc(sizeof(DIR)))) {
		close(fd);
		return NULL;
	}
	memset(dp, 0, sizeof(DIR));
	dp->d_fd = fd;

	return dp;
}
