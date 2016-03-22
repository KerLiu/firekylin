/*
 *    include/firekylin/fs.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _FS_H
#define _FS_H

#include <sys/types.h>
#include <firekylin/sched.h>


struct fs_operation{
	struct super *(*read_super)(struct super *super);
	struct super *(*write_super)(struct super *super);
	struct inode *(*read_inode)(struct inode *inode);
	struct inode *(*write_inode)(struct inode *inode);
};

#define BUF_SIZE	1024
struct buffer {
	dev_t 		b_dev;
	unsigned int    b_block;
	unsigned short  b_flag;
	unsigned short  b_count;
	unsigned char   b_data[BUF_SIZE];
	struct buffer * b_hash_prev;
	struct buffer * b_hash_next;
	struct buffer * b_free_prev;
	struct buffer * b_free_next;
	struct task   * b_wait;
};

/* Bits of buffer->b_flag */
#define B_BUSY		0x0001
#define B_VALID 	0x0002
#define B_DIRTY 	0x0004

#define NR_ZONE		9
struct inode {
	dev_t 		i_dev;
	ino_t 		i_ino;
	mode_t 		i_mode;
	nlink_t 	i_nlink;
	uid_t 		i_uid;
	gid_t 		i_gid;
	dev_t		i_rdev;
	off_t 		i_size;
	time_t 		i_atime;
	time_t 		i_mtime;
	time_t 		i_ctime;
	unsigned int 	i_zone[NR_ZONE];
	unsigned short 	i_flag;
	unsigned short 	i_count;
	struct task * 	i_wait;
};

/* Bits of inode->i_flag */
#define I_BUSY		0x0001
#define I_VALID		0x0002
#define I_DIRTY		0x0004
#define I_MOUNT		0x0008

struct super {
	dev_t          s_dev;
	unsigned short s_inodes;
	unsigned short s_zones;
	unsigned short s_imap_blocks;
	unsigned short s_zmap_blocks;
	unsigned short s_first_data_zone;
	unsigned short s_log_zone_size;
	unsigned long  s_max_size;
	unsigned short s_flag;
	unsigned short s_count;
	struct inode * s_imount;
	struct task  * s_wait;
};

/* Bits of super->s_flag */
#define S_BUSY		0x0001
#define S_VALID		0x0002
#define S_DIRTY		0x0004
#define S_WRITEABLE	0x0008

struct file {
	unsigned long  f_mode;
	unsigned long  f_count;
	unsigned long  f_pos;
	struct inode * f_inode;
};

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

#define NAME_LEN	14
struct dir_entry {
	unsigned short ino;
	char name[NAME_LEN];
};

#define INODES_PER_BLOCK	(1024/sizeof(struct minix1_inode))

extern struct buffer * bread(dev_t dev, long block);
extern void            brelse(struct buffer * buf);

extern struct super  * get_super(dev_t dev);
extern void            put_super(struct super *super);

extern struct inode  * iget(dev_t dev, ino_t ino);
extern struct inode  * ilock(struct inode *inode);
extern struct inode  * iunlock(struct inode *inode);
extern struct inode  * idup(struct inode *inode);
extern struct inode  * namei(char *path, char **basename);
extern void            iput(struct inode *inode);

extern struct inode  * minix1_read_inode(struct inode * inode);
extern struct inode  * minix1_write_inode(struct inode * inode);
extern struct inode  * minix1_look_up(struct inode * inode,char *filename);
extern int             minix1_alloc_block(dev_t dev);
extern void            minix1_free_block(dev_t dev,int block);
extern ino_t           minix1_alloc_inode(dev_t dev);
extern void            minix1_free_inode(dev_t dev,ino_t ino);
extern int             minix1_rbmap(struct inode *inode, int block);
extern int             minix1_wbmap(struct inode *inode, int block);

#endif
