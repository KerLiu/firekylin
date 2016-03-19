/*
 *    fs/super.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>
#include <errno.h>

#define NR_SUPER	4
struct super super_table[NR_SUPER];

static unsigned long super_busy = 0;
static struct task * super_wait = NULL;

static inline void lock_super_table(void)
{
	irq_lock();
	while (super_busy)
		sleep_on(&super_wait);
	super_busy = 1;
	irq_unlock();
}

static inline void unlock_super_table(void)
{
	irq_lock();
	super_busy = 0;
	wake_up(&super_wait);
	irq_unlock();
}

static inline void lock_super(struct super *sb)
{
	irq_lock();
	while (sb->s_flag & S_BUSY)
		sleep_on(&sb->s_wait);
	sb->s_flag |= S_BUSY;
	irq_unlock();
}

static inline void unlock_super(struct super *sb)
{
	irq_lock();
	sb->s_flag &= ~S_BUSY;
	wake_up(&sb->s_wait);
	irq_unlock();
}

static void minix1_read_super(struct super *super)
{
	struct minix1_super *m1_super;
	struct buffer *buf;

	buf = bread(super->s_dev, 1);

	m1_super = (struct minix1_super *) (buf->b_data);
	super->s_inodes = m1_super->s_ninodes;
	super->s_zones = m1_super->s_nzones;
	super->s_imap_blocks = m1_super->s_imap_blocks;
	super->s_zmap_blocks = m1_super->s_zmap_blocks;
	super->s_first_data_zone = m1_super->s_firstdatazone;
	super->s_log_zone_size = m1_super->s_log_zone_size;
	super->s_max_size = m1_super->s_max_size;

	brelse(buf);
}

struct super * get_super(dev_t dev)
{
	struct super *super = super_table;

	lock_super_table();
	while (super < super_table + NR_SUPER) {
		if (super->s_dev == dev) {
			super->s_count++;
			unlock_super_table();
			lock_super(super);
			return super;
		}
		super++;
	}
	panic("Super Block NOT Load %x", dev);
	return NULL;
}

void put_super(struct super *super)
{
	if (!super)
		panic("put NULL super block");

	if (--super->s_count < 0)
		panic("put free super block");

	unlock_super(super);

}

//void dump_super(void)
//{
//	struct super *super;
//
//	for (super = super_table; super < super_table + NR_SUPER; super++) {
//		if (super->s_dev) {
//			printk("s_dev    %x\t", super->s_dev);
//			printk("s_inodes %d\t", super->s_inodes);
//			printk("s_zones  %d\t", super->s_zones);
//			printk("s_imap   %d\t", super->s_imap_blocks);
//			printk("s_zmap   %d\t", super->s_zmap_blocks);
//			printk("s_fdzone %d\t", super->s_first_data_zone);
//			printk("s_lzsize %d\t", super->s_log_zone_size);
//			printk("s_msize  %x\t", super->s_max_size);
//			printk("s_flag   %x\t", super->s_flag);
//			printk("s_count  %d\n", super->s_count);
//		}
//	}
//}

int sys_mount(char *dev_name, char *dir_name, long ro_flag)
{
	return -ERROR;
}

int sys_umount(char *dev_name)
{
	return -ERROR;
}

void mount_root(void)
{
	extern struct inode *root_inode;
	struct super *super = super_table;
	struct task *task = CURRENT_TASK();

	super->s_dev = ROOT_DEV;
	minix1_read_super(super);
	root_inode = iunlock(iget(ROOT_DEV, 1));
	task->pwd = iunlock(idup(root_inode));
}
