/*
 *    driver/driver.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>

struct char_dev *char_table[DEV_CHAR_MAX];
struct blk_dev  *blk_table[DEV_BLK_MAX];

void read_block(struct buffer* buf)
{
	int major = MAJOR(buf->b_dev);
	if (major > DEV_BLK_MAX || !blk_table[major])
		panic("dev %x not exsit", buf->b_dev);
	blk_table[major]->read(buf);
}

void write_block(struct buffer* buf)
{
	int major = MAJOR(buf->b_dev);
	if (major > DEV_BLK_MAX || !blk_table[major])
		panic("dev %x not exsit", buf->b_dev);
	blk_table[major]->write(buf);
}
