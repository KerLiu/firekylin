/*
 *    fs/minix/inode.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/fs.h>
#include <sys/stat.h>

static void m1_read_inode(struct inode *inode, struct minix1_inode *m1)
{
	inode->i_mode = m1->i_mode;
	inode->i_nlink = m1->i_nlinks;
	inode->i_uid = m1->i_uid;
	inode->i_gid = m1->i_gid;
	inode->i_size = m1->i_size;
	inode->i_atime = inode->i_mtime = inode->i_ctime = m1->i_time;
	if (S_ISCHR(inode->i_mode) || S_ISBLK(inode->i_mode))
		inode->i_rdev = m1->i_zone[0];
	else {
		inode->i_zone[0] = m1->i_zone[0];
		inode->i_zone[1] = m1->i_zone[1];
		inode->i_zone[2] = m1->i_zone[2];
		inode->i_zone[3] = m1->i_zone[3];
		inode->i_zone[4] = m1->i_zone[4];
		inode->i_zone[5] = m1->i_zone[5];
		inode->i_zone[6] = m1->i_zone[6];
		inode->i_zone[7] = m1->i_zone[7];
		inode->i_zone[8] = m1->i_zone[8];
	}
	inode->i_flag |= I_VALID;
}

static void m1_write_inode(struct inode *inode, struct minix1_inode *m1)
{
	m1->i_mode = inode->i_mode;
	m1->i_uid = inode->i_uid;
	m1->i_gid = inode->i_gid;
	m1->i_nlinks = inode->i_nlink;
	m1->i_size = inode->i_size;
	m1->i_time = inode->i_ctime;
	if (S_ISCHR(inode->i_mode) || S_ISBLK(inode->i_mode))
		m1->i_zone[0] = inode->i_rdev;
	else {
		m1->i_zone[0] = inode->i_zone[0];
		m1->i_zone[1] = inode->i_zone[1];
		m1->i_zone[2] = inode->i_zone[2];
		m1->i_zone[3] = inode->i_zone[3];
		m1->i_zone[4] = inode->i_zone[4];
		m1->i_zone[5] = inode->i_zone[5];
		m1->i_zone[6] = inode->i_zone[6];
		m1->i_zone[7] = inode->i_zone[7];
		m1->i_zone[8] = inode->i_zone[8];
	}
	inode->i_flag &= ~I_DIRTY;
}

static struct inode * __minix1_rw_inode(struct inode *inode, int rw)
{
	struct super *super;
	struct buffer *buf;
	struct minix1_inode *m1;
	long block;

	super = get_super(inode->i_dev);

	block = 2 + super->s_imap_blocks + super->s_zmap_blocks
			+ (inode->i_ino - 1) / INODES_PER_BLOCK;
	put_super(super);

	buf = bread(inode->i_dev, block);
	m1 = (struct minix1_inode *) buf->b_data
			+ (inode->i_ino - 1) % INODES_PER_BLOCK;
	if (rw) {
		m1_read_inode(inode, m1);
	} else {
		m1_write_inode(inode, m1);
		buf->b_flag|=B_DIRTY;
	}
	brelse(buf);
	return inode;
}

struct inode * minix1_read_inode(struct inode * inode)
{
	return __minix1_rw_inode(inode, 1);
}

struct inode * minix1_write_inode(struct inode * inode)
{
	return __minix1_rw_inode(inode, 0);
}

struct inode * minix1_look_up(struct inode *dir_inode, char *filename)
{
	struct buffer *buf;
	struct inode *inode = NULL;
	struct dir_entry *de;
	int entries;
	int ino;

	if (!dir_inode)
		panic("minix1_look_up:dir_inode is NULL");
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
	iput(dir_inode);
	brelse(buf);
	return NULL;
}

static int __minix1_bmap(struct inode *inode, int block, int create)
{
	struct buffer *buffer;
	int res;

	if (block < 7) {
		if (create && (!inode->i_zone[block])) {
			inode->i_zone[block] = minix1_alloc_block(inode->i_dev);
			inode->i_ctime = current_time();
			inode->i_flag |= I_DIRTY;
			printk("wbmp_come here");
		}
		return inode->i_zone[block];
	}

	block -= 7;
	if (block < 512) {
		if (create && !inode->i_zone[7]) {
			inode->i_zone[7] = minix1_alloc_block(inode->i_dev);
			inode->i_ctime = current_time();
			inode->i_flag |= I_DIRTY;
		}
		if (!inode->i_zone[7])
			return 0;
		buffer = bread(inode->i_dev, inode->i_zone[8]);
		res = *((short*) (buffer->b_data + block * 2));
		if (create && !res) {
			res = minix1_alloc_block(inode->i_dev);
			*((short*) (buffer->b_data + block * 2)) = res;
			buffer->b_flag |= B_DIRTY;
		}
		brelse(buffer);
		return res;
	}

	return 0;
}

int minix1_rbmap(struct inode *inode, int block)
{
	return __minix1_bmap(inode, block, 0);
}

int minix1_wbmap(struct inode *inode, int block)
{
	return __minix1_bmap(inode, block, 1);
}
