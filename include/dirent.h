/*
 *    include/dirent.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _DIRENT_H
#define _DIRENT_H

#include <sys/types.h>

struct dirent{
	ino_t  d_ino;
	char * d_name;
};

#endif
