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

struct buffer * find_entry(struct inode *dir_inode, char *filename, long *off)
{
	struct buffer *buf;
	struct inode *inode = NULL;
	struct dir_entry *de;
	int entries;
	int ino;

	if (!dir_inode)
		panic("find_entry:dir_inode is NULL");

	entries = dir_inode->i_size / sizeof(struct dir_entry);
	buf = bread(dir_inode->i_dev, dir_inode->i_zone[0]);
	de = (struct dir_entry *) buf->b_data;
	while (entries--) {
		if (!strncmp(de->name, filename, NAME_LEN)) {
			//ino = de->ino;
			//brelse(buf);
			//inode = iget(dir_inode->i_dev, de->ino);
			//iput(dir_inode);
			//return inode;
		}
		de++;
	}
	brelse(buf);
	return NULL;
}

void add_entry(struct inode *dir_inode, char *name)
{
	struct buffer *buf;
	struct inode *inode;
	struct dir_entry *de;
	int size;
	ino_t ino;

	if (!dir_inode)
		panic("find_entry:dir_inode is NULL");

	if (!(ino = minix1_alloc_inode(dir_inode->i_dev)))
		panic("add entry :inode use over");

	inode = iget(dir_inode->i_dev, ino);
	inode->i_nlink = 1;
	iput(inode);

	for (int i = 0; i < 7 + 512; i++) {
		buf = bread(dir_inode->i_dev,minix1_wbmap(dir_inode, i));
		if (!buf)
			panic("add_entry:can not read buf");
		de = (struct dir_entry *) buf->b_data;
		for (int j = 0; j < 1024 / sizeof(struct dir_entry); j++) {
			if (de->name[0]) {
				de++;
				continue;
			}
			de->ino = ino;
			strncpy(de->name, name, NAME_LEN);
			buf->b_flag |= B_DIRTY;
			size = i * 1024 + j * sizeof(struct dir_entry);
			if (size > dir_inode->i_size) {
				dir_inode->i_size = size;
				dir_inode->i_flag |= I_DIRTY;
			}
			brelse(buf);
			return  ;
		}
		brelse(buf);
	}
}

int create_file(char *filename)
{
	struct inode *inode;
	char *basename;

	if (!(inode = namei(filename, &basename)))
		return -ENOENT;

	return 0;
}

int sys_create(char *filename, mode_t mode, long arg)
{
	return -EIO;
}

int sys_link(char *name1, char *name2)
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
