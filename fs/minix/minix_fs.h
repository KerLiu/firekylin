/*
 *    fs/minix/minix_fs.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _MINIX_FS_H
#define _MINIX_FS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <firekylin/kernel.h>
#include <firekylin/fs.h>

struct minix1_super {
	unsigned short s_ninodes;
	unsigned short s_nzones;
	unsigned short s_imap_blocks;
	unsigned short s_zmap_blocks;
	unsigned short s_firstdatazone;
	unsigned short s_log_zone_size;
	unsigned long  s_max_size;
	unsigned short s_magic;
};

struct minix1_inode {
	unsigned short i_mode;
	unsigned short i_uid;
	unsigned long  i_size;
	unsigned long  i_time;
	unsigned char  i_gid;
	unsigned char  i_nlinks;
	unsigned short i_zone[9];
};

#define INODES_PER_BLOCK	(1024/sizeof(struct minix1_inode))

extern int minix1_read_super(struct super *super);
extern int minix1_read_inode(struct inode * inode);
extern int minix1_write_inode(struct inode * inode);
extern int minix1_look_up(struct inode *dir_inode, char *filename,
		struct inode **res_inode);
extern int minix1_alloc_block(dev_t dev);
extern int minix1_free_block(dev_t dev,int block);
extern int minix1_alloc_inode(dev_t dev);
extern int minix1_free_inode(dev_t dev,ino_t ino);
extern int minix1_rbmap(struct inode *inode, int block);
extern int minix1_wbmap(struct inode *inode, int block);
extern int minix1_file_read(struct inode *inode, char * buf, size_t size,
		off_t off, int rw_flag);
extern int minix1_file_write(struct inode *inode, char * buf, size_t size,
		off_t off, int rw_flag);

#endif
