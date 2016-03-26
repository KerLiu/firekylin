/*
 *    include/dirent.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _DIRENT_H
#define _DIRENT_H

#include <sys/types.h>

#define NAME_LEN	14
struct dirent {
	unsigned short d_ino;
	char           d_name[NAME_LEN];
};

#define BUF_SIZE	1024
typedef struct {
	int  d_fd;
	int  d_size;
	int  d_off;
	char d_buf[BUF_SIZE];
} DIR;

DIR 	*opendir(char *filename);
struct dirent *readdir(DIR *dp);
void 	rewinddir(DIR *dp);
int 	closedir(DIR *dp);

#endif
