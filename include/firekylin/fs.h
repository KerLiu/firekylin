/*
 *    include/firekylin/fs.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _FS_H
#define _FS_H

#include <sys/types.h>
#include <firekylin/sched.h>

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
	dev_t 		     i_dev;
	ino_t 		     i_ino;
	mode_t 		     i_mode;
	nlink_t 	     i_nlink;
	uid_t 		     i_uid;
	gid_t 		     i_gid;
	dev_t		     i_rdev;
	off_t 		     i_size;
	time_t 		     i_atime;
	time_t 		     i_mtime;
	time_t 		     i_ctime;
	unsigned int 	     i_zone[NR_ZONE];
	struct fs_operation *i_op;
	unsigned short 	     i_flag;
	unsigned short 	     i_count;
	struct task         *i_wait;
};

/* Bits of inode->i_flag */
#define I_BUSY		0x0001
#define I_VALID		0x0002
#define I_DIRTY		0x0004
#define I_MOUNT		0x0008

struct super {
	dev_t                s_dev;
	unsigned short       s_inodes;
	unsigned short       s_zones;
	unsigned short       s_imap_blocks;
	unsigned short       s_zmap_blocks;
	unsigned short       s_first_data_zone;
	unsigned short       s_log_zone_size;
	unsigned long        s_max_size;
	unsigned short       s_flag;
	unsigned short       s_count;
	struct fs_operation *s_op;
	struct inode        *s_imount;
	struct task         *s_wait;
};

/* Bits of super->s_flag */
#define S_BUSY		0x0001
#define S_VALID		0x0002
#define S_DIRTY		0x0004
#define S_WRITEABLE	0x0008

struct fs_operation{
	int (*super_read)(struct super *super);
	int (*super_write)(struct super *super);
	int (*inode_read)(struct inode *inode);
	int (*inode_write)(struct inode *inode);
	int (*look_up)(struct inode *inode, char *filename, struct inode **res);
	int (*file_read)(struct inode *inode, char *buf, size_t size,
			off_t offset, int rw_flag);
	int (*file_write)(struct inode *inode, char *buf, size_t size,
			off_t offset, int rw_flag);
	int (*mknod)(struct inode *inode, char *name, struct inode **res_inode);
	int (*rename)(struct inode *inode,char *old,char *new);
	int (*remove)(struct inode *inode,char *name);
	int (*link)(struct inode *dir_inode,char *name,struct inode *inode);
};

struct file {
	unsigned long  f_mode;
	unsigned long  f_count;
	unsigned long  f_pos;
	struct inode * f_inode;
};

#define NAME_LEN	14
struct dir_entry {
	unsigned short ino;
	char name[NAME_LEN];
};

extern struct fs_operation minix_fs_operation;

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

#endif
