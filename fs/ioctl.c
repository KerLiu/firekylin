/*
 *    fs/fcntl.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <errno.h>

static int char_ioctl(dev_t dev, int cmd, long arg)
{
	int major = MAJOR(dev);

	if (major > DEV_CHAR_MAX || !char_table[major]) {
		printk("char dev not exist:%x", dev);
		return -ENODEV;
	}
	if (char_table[major]->ioctl)
		return char_table[major]->ioctl(dev, cmd, arg);
	return -1;
}

static int blk_ioctl(dev_t dev, int cmd, long arg)
{
	int major = MAJOR(dev);

	if (major > DEV_BLK_MAX || !blk_table[major]) {
		printk("block dev not exist:%x", dev);
		return -ENODEV;
	}
	if (blk_table[major]->ioctl)
		return blk_table[major]->ioctl(dev, cmd, arg);
	return -1;
}

int sys_ioctl(unsigned int fd, int cmd, long arg)
{
	struct file *file;
	struct inode *inode;
	int res;

	if (fd > NR_OPEN || !(file = (CURRENT_TASK() )->file[fd]))
		return -EBADF;

	inode = idup(file->f_inode);

	res = -EINVAL;
	if (S_ISCHR(inode->i_mode))
		res = char_ioctl(inode->i_zone[0], cmd, arg);
	if (S_ISBLK(inode->i_mode))
		res = blk_ioctl(inode->i_zone[0], cmd, arg);

	iput(inode);
	return res;
}
