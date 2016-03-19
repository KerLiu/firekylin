/*
 *    fs/stat.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/fs.h>
#include <sys/stat.h>
#include <errno.h>

static void copy_stat(struct inode *inode ,struct stat *statbuf)
{
	statbuf->st_dev = inode->i_dev;
	statbuf->st_ino = inode->i_ino;
	statbuf->st_mode = inode->i_mode;
	statbuf->st_nlink = inode->i_link;
	statbuf->st_uid = inode->i_uid;
	statbuf->st_gid = inode->i_gid;
	statbuf->st_rdev = inode->i_zone[0];
	statbuf->st_size = inode->i_size;
	statbuf->st_otime = inode->i_otime;
	statbuf->st_atime = inode->i_atime;
	statbuf->st_mtime = inode->i_mtime;
	statbuf->st_ctime = inode->i_ctime;
}

int sys_stat(char *filename, struct stat *statbuf)
{
	struct inode *inode;

	if (!(inode = namei(filename, NULL)))
		return -ENOENT;

	copy_stat(inode,statbuf);
	iput(inode);

	return 0;
}

int sys_fstat(int fd, struct stat *statbuf)
{
	struct inode *inode;
	struct task *task=CURRENT_TASK();

	if(fd>NR_OPEN || !(task->file[fd]))
		return -EBADF;

	inode=idup(task->file[fd]->f_inode);
	copy_stat(inode,statbuf);
	iput(inode);
	return 0;
}

int sys_chown(char *filename, uid_t uid, gid_t gid)
{
	struct inode *inode;

	if ((CURRENT_TASK() )->uid != 0)
		return -EPERM;
	if (!(inode = namei(filename, NULL)))
		return -ENOENT;

	inode->i_uid = uid;
	inode->i_gid = gid;
	inode->i_flag |= I_DIRTY;
	iput(inode);
	return 0;
}

int sys_chmod(char *filename, mode_t mode)
{
	struct inode *inode;
	struct task *current;

	if (!(inode = namei(filename, NULL)))
		return -ENOENT;

	current=CURRENT_TASK();
	if (current->uid != inode->i_uid && current->uid != 0) {
		iput(inode);
		return -EACCESS;
	}

	inode->i_mode = (inode->i_mode & ~07777) | (mode & 07777);
	inode->i_flag |= I_DIRTY;
	iput(inode);
	return 0;
}

int sys_utime(char *filename, struct utimebuf *timebuf)
{
	struct inode *inode;

	if (!(inode = namei(filename, NULL)))
		return -ENOENT;

	if (timebuf) {
		inode->i_otime = timebuf->otime;
		inode->i_ctime = timebuf->ctime;
	}
	inode->i_atime = inode->i_mtime = current_time();
	inode->i_flag |= I_DIRTY;
	iput(inode);
	return 0;
}
