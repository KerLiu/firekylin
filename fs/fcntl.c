/*
 *    fs/fcntl.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <errno.h>

extern int sys_close(int fd);

static int dupfd(int fd, unsigned int arg)
{
	struct task *current;

	if (arg > NR_OPEN)
		return -EBADF;

	if (arg)
		sys_close(arg);
	current = CURRENT_TASK();
	while (arg < NR_OPEN) {
		if (current->file[arg]) {
			arg++;
			continue;
		}
		current->file[arg] = current->file[fd];
		current->file[fd]->f_count++;
		return arg;
	}

	return -EMFILE;
}

static int do_lseek(struct file *file, long off, int where)
{
	switch (where) {
		case F_SEEKSET:
			if (off < 0)
				return -EINVAL;
			file->f_pos = off;
			break;
		case F_SEEKCUR:
			if (file->f_pos + off < 0)
				return -EINVAL;
			file->f_pos += off;
			break;
		case F_SEEKEND:
			if (file->f_inode->i_size + off < 0)
				return -EINVAL;
			file->f_pos = file->f_inode->i_size + off;
			break;
		default:
			return -EINVAL;
	}
	return file->f_pos;
}

int sys_fcntl(unsigned int fd, unsigned int cmd, unsigned long arg)
{
	struct file *file;

	if (fd > NR_OPEN || !(file = (CURRENT_TASK() )->file[fd]))
		return -EBADF;

	switch (cmd) {
		case F_DUPFD:
			return dupfd(fd, arg);
		case F_SEEKSET:
		case F_SEEKCUR:
		case F_SEEKEND:
			return do_lseek(file, arg, cmd);
		default:
			return -EIO;
	}
}
