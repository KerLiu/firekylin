/*
 *    fs/minix/file.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include "minix_fs.h"

int minix1_file_read(struct inode *inode, char * buf, size_t size, off_t off,
		int rw_flag)
{
	struct buffer *bh;
	int block, chars, left;

	if (off >= inode->i_size)
		return 0;
	if (off + size >= inode->i_size)
		size = inode->i_size - off;

	left = size;
	while (left > 0) {
		bh = bread(inode->i_dev, minix1_rbmap(inode, off / 1024));
		if (!bh) {
			panic("read file error");
		}
		chars = min(left, 1024 - off % 1024);
		memcpy(buf, bh->b_data + off % 1024, chars);
		brelse(bh);
		buf += chars;
		off += chars;
		left -= chars;
	}
	return size - left;
}

int minix1_file_write(struct inode *inode, char * buf, size_t size, off_t off,
		int rw_flag)
{
	struct buffer *bh;
	int left, chars;

	left = size;
	while (left) {
		bh = bread(inode->i_dev, minix1_wbmap(inode, off / 1024));
		if (!bh) {
			panic("write file error");
		}
		chars = min((1024 - off % 1024), left);
		memcpy(bh->b_data + off % 1024, buf, chars);
		off += chars;
		left -= chars;
	}
	brelse(bh);
	return size - left;
}
