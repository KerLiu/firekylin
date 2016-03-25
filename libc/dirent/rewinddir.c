/*
 *	libc/dirent/rewinddir.c
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

void rewinddir(DIR * dir)
{
	if (!dir) {
		errno = EBADF;
		return;
	}
	dir->d_size = dir->d_off = 0;
	lseek(dir->d_fd,0L,SEEK_SET);
}
