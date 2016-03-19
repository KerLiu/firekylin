/*
 *    fs/create.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/sched.h>
#include <errno.h>

int sys_pipe(int fd[2])
{
	return -ERROR;
}

int pipe_open(struct inode *inode)
{
	return 0;
}

int read_pipe(struct inode *inode,char *buf,size_t size)
{
	return 0;
}

int write_pipe(struct inode *inode,char *buf,size_t size)
{
	return 0;
}

int pipe_close(struct inode *inode)
{
	return 0;
}

int pipe_ioctl(struct inode *inode ,int cmd ,long arg)
{
	switch(cmd){
		default:
		return 0;
	}
}
