/*
 *	libc/dirent/readdir.c
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

struct dirent * readdir(DIR * dir)
{
	struct dirent * ptr;

	if (!dir) {
		errno = EBADF;
		return NULL;
	}

	while (1) {
		if (dir->d_size <= dir->d_off) {
			dir->d_off = 0;
			dir->d_size = read(dir->d_fd,dir->d_buf,BUF_SIZE);
		}
		if (dir->d_size <= 0)
			return NULL;

		ptr = (struct dirent *) (dir->d_buf +dir->d_off );
		dir->d_off += sizeof (struct dirent);
		if (!ptr->d_ino)
			continue;
		return ptr;
	}
}
