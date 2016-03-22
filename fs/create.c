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

void add_entry(struct inode *inode, char *name, ino_t ino)
{
	struct buffer *buf;
	struct dir_entry *de;
	int size;

	if (!inode)
		panic("find_entry:inode is NULL");
	printk("<8-1>");
	for (int i = 0; i < 7 + 512; i++) {
		printk("<8-2>");
		buf = bread(inode->i_dev, minix1_wbmap(inode, i));
		if (!buf)
			panic("add_entry:can not read buf");
		printk("<8-3>");
		de = (struct dir_entry *) buf->b_data;
		for (int j = 0; j < 1024 / sizeof(struct dir_entry); j++) {
			if (de->ino) {
				de++;
				continue;
			}
			printk("<8-4>");
			de->ino = ino;
			strncpy(de->name, name, NAME_LEN);
			buf->b_flag |= B_DIRTY;
			printk("<8-5>");
			size = i * 1024 + j * sizeof(struct dir_entry);
			printk("<8-6>");
			if (size > inode->i_size) {
				inode->i_size = size;
				inode->i_flag |= I_DIRTY;
				minix1_write_inode(inode);
			}
			printk("<8-7>");
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
	if ((inode = minix1_look_up(dir_inode, basename))) {
		iput(dir_inode);
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

	printk("<1>");
	if (!(dir_inode = namei(pathname, &basename)))
		return -ENOENT;

	if (!*basename) {
		iput(dir_inode);
		return -EINVAL;
	}
	printk("<2>%s",basename);
	if ((inode = minix1_look_up(dir_inode, basename))) {
		iput(dir_inode);
		iput(inode);
		return -EEXIST;
	}
	printk("<3>");
	ino = minix1_alloc_inode(dir_inode->i_dev);
	printk("<4>%d",ino);
	if (!ino) {
		iput(dir_inode);
		return -ENOSPACE;
	}
	inode = iget(dir_inode->i_dev, ino);
	if (!inode) {
		iput(dir_inode);
		return -EAGAIN;
	}
	printk("<5>");
	inode->i_mode = S_IFDIR | 0777;
	inode->i_nlink = 1;
	inode->i_ctime = current_time();
	inode->i_flag |= I_DIRTY;
	inode->i_zone[0]=0;
	printk("<6>");
	add_entry(dir_inode, basename, ino);
	s_te[0].ino=inode->i_ino;
	s_te[1].ino=dir_inode->i_ino;
	iput(dir_inode);
	printk("<7>");
	struct buffer* buf=bread(inode->i_dev,minix1_wbmap(inode,0));
	//if(!buf)
		//printk("warning");
	memcpy(buf->b_data,s_te,sizeof(struct dir_entry)*2);
	buf->b_flag|=B_DIRTY;
	brelse(buf);
	iput(inode);
	printk("<8>");
	return 0;
}

int sys_create(char *old, char *new)
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

int sys_link()
{
	return -ERROR;
}
