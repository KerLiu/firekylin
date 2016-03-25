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
	super->s_op=&minix_fs_operation;
	super->s_op->super_read(super);
	root_inode = iunlock(iget(ROOT_DEV, 1));
	task->pwd = iunlock(idup(root_inode));
}
