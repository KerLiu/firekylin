/*
 *    fs/open.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>
#include <sys/stat.h>
#include <errno.h>

#define NR_FILE 	64

struct file file_table[NR_FILE];

int sys_access(char *filename, int mode)
{
	return -ERROR;
}

int char_open(dev_t dev)
{
	int major = MAJOR(dev);

	if (major > DEV_CHAR_MAX || !char_table[major]) {
		printk("char dev not exist:%x", dev);
		return -ENODEV;
	}
	if (char_table[major]->open)
		return char_table[major]->open(dev);
	return 0;
}

int blk_open(dev_t dev)
{
	int major = MAJOR(dev);

	if (major > DEV_BLK_MAX || !blk_table[major]) {
		printk("block dev not exist:%x", dev);
		return -ENODEV;
	}
	if (blk_table[major]->open)
		return blk_table[major]->open(dev);
	return 0;
}

int sys_open(char *path, int flag)
{
	int fd;
	struct file *file;
	struct inode *inode;
	struct task *current = CURRENT_TASK();

	for (fd = 0; fd < NR_OPEN; fd++) {
		if (!current->file[fd])
			break;
	}
	if (fd >= NR_OPEN)
		return -EINVAL;

	for (file = file_table; file < file_table + NR_FILE; file++)
		if (!file->f_count)
			break;
	if (file >= file_table + NR_FILE)
		return -EINVAL;

	if (!(inode = namei(path, NULL)))
		return -ENOENT;
	iunlock(inode);
	file->f_count = 1;
	file->f_inode = inode;
	file->f_pos = 0;
	file->f_mode = flag;

	current->file[fd] = file;

	return fd;
}

int sys_chdir(char *path)
{
	struct inode *inode;
	struct task *current = CURRENT_TASK();

	if (!(inode = namei(path, NULL)))
		return -ENOENT;
	if (!(S_ISDIR(inode->i_mode))) {
		iput(inode);
		return -ENOTDIR;
	}

	iput(current->pwd);
	current->pwd = inode;
	iunlock(inode);
	return 0;
}
