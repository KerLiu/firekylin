/*
 *    fs/inode.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>

static unsigned long inode_busy = 0;
static struct task * inode_wait = NULL;

#define NR_INODE	32
struct inode inode_table[NR_INODE];
struct inode *root_inode;

static inline void lock_inode_table(void)
{
	irq_lock();
	while (inode_busy)
		sleep_on(&inode_wait);
	inode_busy = 1;
	irq_unlock();
}

static inline void unlock_inode_table(void)
{
	irq_lock();
	inode_busy = 0;
	wake_up(&inode_wait);
	irq_unlock();
}

struct inode * ilock(struct inode * inode)
{
	if (!inode) {
		panic("ilock:inode is null");
	}
	irq_lock();
	while (inode->i_flag & I_BUSY)
		sleep_on(&inode->i_wait);
	inode->i_flag |= I_BUSY;
	irq_unlock();
	return inode;
}

struct inode * iunlock(struct inode * inode)
{
	if (!inode) {
		panic("iunlock:inode is null");
	}
	irq_lock();
	inode->i_flag &= ~I_BUSY;
	wake_up(&inode->i_wait);
	irq_unlock();
	return inode;
}

struct inode * idup(struct inode *inode)
{
	if (!inode) {
		panic("idup:inode is null");
	}
	ilock(inode);
	inode->i_count++;
	return inode;
}

struct inode * iget(dev_t dev, ino_t ino)
{
	struct inode * inode;

	lock_inode_table();
repeat:
	if (!dev)
		goto no_dev;
	inode = inode_table;
	while (inode < inode_table + NR_INODE) {
		if (inode->i_dev == dev && inode->i_ino == ino) {
			inode->i_count++;
			unlock_inode_table();
			ilock(inode);
			return inode;
		}
		inode++;
	}
no_dev:
	inode = inode_table;
	while (inode < inode_table + NR_INODE) {
		if (!inode->i_count) {
			inode->i_count++;
			unlock_inode_table();
			ilock(inode);
			if(inode->i_flag&I_DIRTY){
				minix1_write_inode(inode);
				iunlock(inode);
				goto repeat;
			}
			inode->i_dev = dev;
			inode->i_ino = ino;
			inode->i_flag =I_BUSY ;
			if (dev)
				minix1_read_inode(inode);
			return inode;
		}
		inode++;
	}
	unlock_inode_table();
	return NULL;
}

void iput(struct inode * inode)
{
	if (!inode)
		return;

	if (inode->i_count == 0)
		panic("put_inode:put free inode");

	if (--inode->i_count == 0) {
		minix1_write_inode(inode);
	}
	iunlock(inode);
}

struct inode *namei(char *filepath, char **basename)
{
	struct inode *inode;
	char name[NAME_LEN + 1];
	char *n;

	if (*filepath == '/') {
		filepath++;
		inode = idup(root_inode);
	} else
		inode = idup((CURRENT_TASK() )->pwd);

	while (*filepath) {
		if(basename)
			*basename=filepath;

		for (int i = 0; i < NAME_LEN; i++) {
			if (*filepath == 0 || *filepath == '/') {
				name[i] = 0;
				break;
			}
			name[i] = *filepath++;
		}
		if (!*filepath && basename)
			return inode;

		if (*filepath == '/')
			filepath++;

		if (!(inode = minix1_look_up(inode, name))) {
			return NULL;
		}
	}
	return inode;
}

void sync_inode()
{
	struct inode *inode = inode_table;

	while (inode < inode_table + NR_INODE) {
		ilock(inode);
		if (inode->i_flag & I_DIRTY) {
			minix1_write_inode(inode);
			inode->i_flag &= ~I_DIRTY;
		}
		iunlock(inode);
		inode++;
	}
}
