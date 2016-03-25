/*
 *    fs/create.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/fs.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>

int sys_create(char *old, char *new)
{
	return -ENOENT;
}

int sys_rename(char *old, char *new)
{
	return -ENOENT;
}

int sys_remove()
{
	return -ERROR;
}

int sys_link()
{
	return -ERROR;
}
