/*
 *    fs/minix/inode.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include "minix_fs.h"
#include <errno.h>

struct buffer * find_entry(struct inode *dir_inode, char *filename,
		struct dir_entry **res_de)
{
	struct buffer *buf;
	struct inode *inode = NULL;
	struct dir_entry *de;

	for (int i = 0; i < 7 + 512; i++) {
		buf = bread(dir_inode->i_dev, minix1_rbmap(dir_inode, i));
		if (!buf)
			panic("add_entry:can not read buf");

		de = (struct dir_entry *) buf->b_data;
		for (int j = 0; j < 1024 / sizeof(struct dir_entry); j++) {

			if (i * 1024 + j * sizeof(struct dir_entry)
					> dir_inode->i_size) {

				brelse(buf);
				*res_de = NULL;
				return NULL;
			}

			if (!strncmp(de->name, filename, NAME_LEN)) {
				*res_de = de;
				return buf;
			}
			de++;
		}
		brelse(buf);
	}
	*res_de = NULL;
	return NULL;
}

int add_entry(struct inode *inode, struct dir_entry *add_de)
{
	struct buffer *buf;
	struct dir_entry *de;
	int size;

	if (!inode)
		panic("find_entry:inode is NULL");

	for (int i = 0; i < 7 + 512; i++) {

		buf = bread(inode->i_dev, minix1_wbmap(inode, i));
		if (!buf)
			panic("add_entry:can not read buf");

		de = (struct dir_entry *) buf->b_data;
		for (int j = 0; j < 1024 / sizeof(struct dir_entry); j++) {
			if (de->ino) {
				de++;
				continue;
			}

			memcpy(de, add_de, sizeof(struct dir_entry));
			buf->b_flag |= B_DIRTY;

			size = i * 1024 + (j + 1) * sizeof(struct dir_entry);

			if (size > inode->i_size) {
				inode->i_size = size;
				inode->i_flag |= I_DIRTY;
				//minix1_write_inode(inode);
			}

			brelse(buf);
			return 0;
		}
		brelse(buf);
		return 1;
	}
}

int count_entry(struct inode *inode)
{
	struct buffer *buf;
	struct dir_entry *de;
	int count=0;

	for (int i = 0; i < 7 + 512; i++) {
		buf = bread(inode->i_dev, minix1_rbmap(inode, i));
		if (!buf)
			panic("add_entry:can not read buf");

		de = (struct dir_entry *) buf->b_data;
		for (int j = 0; j < 1024 / sizeof(struct dir_entry); j++) {

			if (i * 1024 + j * sizeof(struct dir_entry)
					> inode->i_size) {

				brelse(buf);
				return count;
			}

			if(!de->ino){
				de++;
				continue;
			}
			count++;
			de++;
		}
		brelse(buf);
	}
	return count;
}

int minix1_look_up(struct inode *dir_inode, char *filename,
		struct inode **res_inode)
{
	struct buffer *buf;
	struct inode *inode;
	struct dir_entry *de;
	int ino;

	if (filename[0] == '.' && filename[1] == 0) {
		*res_inode = dir_inode;
		return 0;
	}

	buf = find_entry(dir_inode, filename, &de);

	if (!buf) {
		iput(dir_inode);
		*res_inode = NULL;
		return 1;

	}

	if (de->ino == dir_inode->i_ino) {
		brelse(buf);
		*res_inode = dir_inode;
		return 0;
	}

	inode = iget(dir_inode->i_dev, de->ino);
	if (!inode) {
		brelse(buf);
		*res_inode = NULL;
		return 1;
	}
	brelse(buf);
	iput(dir_inode);
	*res_inode = inode;
	return 0;
}

int minix1_mknod(struct inode *inode, char *name, struct inode **res_inode)
{
	struct buffer *buf;
	struct dir_entry *de;
	struct dir_entry add_de;
	struct inode *new_inode;
	ino_t ino;

	if ((buf = find_entry(inode, name, &de))) {
		brelse(buf);
		*res_inode = NULL;
		return -EEXIST;
	}
	ino = minix1_alloc_inode(inode->i_dev);
	if (!ino)
		return -ENOSPACE;

	new_inode = iget(inode->i_dev, ino);
	if (!new_inode)
		return -EAGAIN;
	add_de.ino = ino;
	strncpy(add_de.name, name, NAME_LEN);
	if ((add_entry(inode, &add_de))) {
		iput(new_inode);
		minix1_free_inode(inode->i_dev, ino);
		*res_inode = NULL;
		return -1;
	}
	*res_inode = new_inode;
	return 0;
}

int minix1_rename(struct inode *inode, char *old, char *new)
{
	struct dir_entry *de;
	struct buffer *buf;

	buf = find_entry(inode, old, &de);
	if (!buf)
		return -ENOENT;
	strncpy(de->name, new, NAME_LEN);
	buf->b_flag |= B_DIRTY;
	brelse(buf);
	return 0;
}

int minix1_remove(struct inode *inode, char *name)
{
	struct dir_entry *de;
	struct buffer *buf;
	struct inode *del_inode;

	buf = find_entry(inode, name, &de);
	if (!buf)
		return -ENOENT;

	del_inode = iget(inode->i_dev, de->ino);

	if (!del_inode) {
		brelse(buf);
		return -EAGAIN;
	}

	if (del_inode->i_count != 1) {
		iput(del_inode);
		brelse(buf);
		return -EBUSY;
	}
	if (S_ISDIR(del_inode->i_mode) && count_entry(del_inode) > 2) {
		iput(del_inode);
		brelse(buf);
		return -ERROR;
	}

	del_inode->i_nlink--;
	iput(del_inode);
	de->ino = 0;
	buf->b_flag |= B_DIRTY;
	brelse(buf);
	return 0;
}
