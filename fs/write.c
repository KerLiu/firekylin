/*
 *    fs/write.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>

static int write_char(dev_t dev, char *buf, off_t off, size_t size)
{
	int major = MAJOR(dev);
	if (major > DEV_CHAR_MAX || !char_table[major])
		panic("dev %x not exsit", dev);

	return (*(char_table[major]->write))(dev, buf, off, size);
}

static int write_blk(int dev, char *buf, long off, size_t size)
{
	struct buffer *bh;
	int block, chars, left;

	left = size;
	while (left) {
		bh = bread(dev, off / 1024);
		if (!bh)
			return -EIO;
		chars = min(left, 1024 - off % 1024);
		memcpy(bh->b_data+off%1024, buf, chars);
		bh->b_flag |= B_DIRTY;
		brelse(bh);
		buf += chars;
		off += chars;
		left -= chars;
	}
	return size - left;
}

static int write_file(struct inode *inode, char * buf, off_t off, int size)
{
	return inode->i_op->file_write(inode,buf,size,off,0);
}

extern int write_pipe(struct inode *inode,char *buf,size_t size);

int sys_write(int fd, char *buf, size_t size)
{
	struct file *file;
	struct inode *inode;
	int res;

	if (fd > NR_OPEN || !(file = (CURRENT_TASK() )->file[fd]))
		return -EBADF;
	if (!(file->f_mode & O_WRITE))
		return -EBADF;

	inode = idup(file->f_inode);
	if (file->f_mode & O_APPEND)
		file->f_pos = inode->i_size;
	switch (inode->i_mode & S_IFMT) {
		case S_IFREG:
			res = write_file(inode, buf, file->f_pos, size);
			break;
		case S_IFDIR:
			res = -EISDIR;
			break;
		case S_IFCHR:
			res = write_char(inode->i_rdev, buf, file->f_pos,
					size);
			break;
		case S_IFBLK:
			res = write_blk(inode->i_rdev, buf, file->f_pos,
					size);
			break;
		case S_IFIFO:
			res= write_pipe(inode,buf,size);
			break;
		default:
			res = -EIO;
	}
	iput(inode);

	if (res > 0)
		file->f_pos += res;

	return res;
}
