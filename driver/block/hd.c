/*
 *    driver/hd.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <firekylin/trap.h>
#include <firekylin/driver.h>
#include <firekylin/hd.h>
#include <firekylin/fs.h>

static struct hd_ctl HD0 = { 0 };

static void hd_out_cmd(int sector, int nr, int cmd)
{
	union {
		char arg[4];
		int sec;
	} sect;
	sect.sec = sector;
	outb(0x1f2, nr);
	outb(0x1f3, sect.arg[0]);
	outb(0x1f4, sect.arg[1]);
	outb(0x1f5, sect.arg[2]);
	outb(0x1f6, (sect.arg[3]&0x0f)+0xe0);
	outb(0x1f7, cmd);
}

static void ide_read(struct buffer *buffer)
{
	irq_lock();
	while (HD0.busy)
		sleep_on(&HD0.wait);
	HD0.busy = 1;
	HD0.cmd = WIN_READ;
	HD0.buf = buffer;
	hd_out_cmd(buffer->b_block * 2, 2, WIN_READ);
	irq_unlock();
}

static void ide_write(struct buffer *buffer)
{
	irq_lock();
	while (HD0.busy)
		sleep_on(&HD0.wait);
	HD0.busy = 1;
	HD0.cmd = WIN_WRITE;
	HD0.buf = buffer;
	hd_out_cmd(buffer->b_block * 2, 2, WIN_WRITE);
	outs(HD_DATA, buffer->b_data, 1024);
	irq_unlock();
}

static void do_hd(struct trapframe *tf)
{
	outb(0xa0, 0x20);
	outb(0x20, 0x20);

	if (HD0.cmd == WIN_READ)
		ins(HD_DATA, HD0.buf->b_data, 1024);
	HD0.busy = 0;
	HD0.buf->b_flag &= ~(B_BUSY|B_DIRTY);
	HD0.buf->b_flag |= B_VALID;
	wake_up(&(HD0.wait));
	wake_up(&(HD0.buf->b_wait));
}

static void hd_identify(void)
{
	short buf[256];
	char tmp;

	hd_out_cmd(0, 0, WIN_IDENTIFY);
	do {
		tmp = inb(HD_STATUS);
	} while ((tmp & (HD_STAT_BUSY | HD_STAT_DRQ)) != HD_STAT_DRQ);
	ins(0x1f0, buf, 512);

	HD0.LBA = buf[60] | buf[61] << 16;
}

static struct blk_dev ide = { "ATA HD", NULL, NULL, ide_read, ide_write, NULL };

void hd_init(void)
{
	hd_identify();
	set_trap_handle(0x2e, do_hd);
	outb(0xa1, inb(0xa1)&0xbf);
	blk_table[DEV_BLK_IDE] = &ide;
}
