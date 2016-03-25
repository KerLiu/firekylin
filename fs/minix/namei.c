/*
 *    fs/minix/inode.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include "minix_fs.h"
#include <errno.h>


struct inode * find_entry(struct inode *dir_inode, char *filename)
{
	struct buffer *buf;
	struct inode *inode = NULL;
	struct dir_entry *de;
	int entries;
	int ino;

	if (!dir_inode)
		panic("find_entry:dir_inode is NULL");
	if (filename[0] == '.' && filename[1] == 0)
		return dir_inode;
	entries = dir_inode->i_size / sizeof(struct dir_entry);

	buf = bread(dir_inode->i_dev, dir_inode->i_zone[0]);

	de = (struct dir_entry *) buf->b_data;
	while (entries--) {
		if (!strncmp(de->name, filename, NAME_LEN)) {
			ino = de->ino;
			brelse(buf);
			if(ino ==dir_inode->i_ino){
				inode=dir_inode;
			}else{
				inode = iget(dir_inode->i_dev, de->ino);
				iput(dir_inode);
			}
			return inode;
		}
		de++;
	}
	brelse(buf);
	return NULL;
}
void add_entry(struct inode *inode, char *name, ino_t ino)
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

			de->ino = ino;
			strncpy(de->name, name, NAME_LEN);
			buf->b_flag |= B_DIRTY;

			size = i * 1024 + j * sizeof(struct dir_entry);

			if (size > inode->i_size) {
				inode->i_size = size;
				inode->i_flag |= I_DIRTY;
				minix1_write_inode(inode);
			}

			brelse(buf);
			return;
		}
		brelse(buf);
	}
}

int sys_create2(char *pathname)
{
	struct inode *dir_inode, *inode;
	char *basename;
	ino_t ino;

	if (!(dir_inode = namei(pathname, &basename)))
		return -ENOENT;

	if (!*basename) {
		iput(dir_inode);
		return -EINVAL;
	}

	dir_inode->i_count++;
	if ((inode = find_entry(dir_inode, basename))) {
		iput(inode);
		return -EEXIST;
	}
	ino = minix1_alloc_inode(dir_inode->i_dev);
	if (!ino) {
		iput(dir_inode);
		return -ENOSPACE;
	}
	inode = iget(dir_inode->i_dev, ino);
	if (!inode) {
		iput(dir_inode);
		return -EAGAIN;
	}
	inode->i_mode = S_IFREG | 0777;
	inode->i_nlink = 1;
	inode->i_ctime = current_time();
	inode->i_flag |= I_DIRTY;
	add_entry(dir_inode, basename, ino);
	iput(dir_inode);
	iput(inode);
	return -ERROR;
}

struct dir_entry s_te[2]={
	{0,".\0\0\0\0\0\0\0\0\0\0\0\0\0"},
	{0,"..\0\0\0\0\0\0\0\0\0\0\0\0"},
};

int sys_mkdir(char *pathname)
{
	struct inode *dir_inode, *inode;
	char *basename;
	ino_t ino;


	if (!(dir_inode = namei(pathname, &basename)))
		return -ENOENT;

	if (!*basename) {
		iput(dir_inode);
		return -EINVAL;
	}

	if ((inode = find_entry(dir_inode, basename))) {
		iput(inode);
		return -EEXIST;
	}

	ino = minix1_alloc_inode(dir_inode->i_dev);

	if (!ino) {
		iput(dir_inode);
		return -ENOSPACE;
	}
	inode = iget(dir_inode->i_dev, ino);
	if (!inode) {
		iput(dir_inode);
		return -EAGAIN;
	}

	inode->i_mode = S_IFDIR | 0777;
	inode->i_nlink = 1;
	inode->i_ctime = current_time();
	inode->i_flag |= I_DIRTY;
	inode->i_zone[0]=0;
	inode->i_size=32;

	add_entry(dir_inode, basename, ino);
	s_te[0].ino=inode->i_ino;
	s_te[1].ino=dir_inode->i_ino;
	iput(dir_inode);

	struct buffer* buf=bread(inode->i_dev,minix1_wbmap(inode,0));
	memcpy(buf->b_data,s_te,sizeof(struct dir_entry)*2);
	buf->b_flag|=B_DIRTY;
	brelse(buf);
	iput(inode);

	return 0;
}
