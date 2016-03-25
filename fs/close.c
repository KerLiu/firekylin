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

//extern int pipe_close(struct inode *inode);

int char_close(dev_t dev)
{
	int major = MAJOR(dev);

	if (major > DEV_CHAR_MAX || !char_table[major])
		panic("char dev not exist:%x", dev);
	if (char_table[major]->close)
		return char_table[major]->close(dev);
	return 0;
}

int blk_close(dev_t dev)
{
	int major = MAJOR(dev);

	if (major > DEV_CHAR_MAX || !blk_table[major])
		panic("block dev not exist:%x", dev);
	if (blk_table[major]->close)
		return blk_table[major]->close(dev);
	return 0;
}

int sys_close(unsigned int fd)
{
	struct file *filp;
	struct inode *inode;
	struct task *current = CURRENT_TASK();

	if (fd > NR_OPEN || !(filp = current->file[fd]))
		return -EINVAL;

	current->file[fd] = NULL;

	inode = filp->f_inode;
	if (--filp->f_count < 0)
		panic("close:file count is 0");
	if (!filp->f_count)
		iput(inode);
	return 0;
}
