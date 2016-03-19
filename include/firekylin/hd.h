/*
 *    include/firekylin/hd.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _HD_H
#define _HD_H

#define HD_DATA		0x1f0	/* _CTL when writing */
#define HD_ERROR	0x1f1	/* see err-bits */
#define HD_PRECOMP   HD_ERROR	/* same address, read=error, write=precomp */
#define HD_NSECTOR	0x1f2	/* nr of sectors to read/write */
#define HD_SECTOR	0x1f3	/* starting sector */
#define HD_LCYL		0x1f4	/* starting cylinder */
#define HD_HCYL		0x1f5	/* high byte of starting cyl */
#define HD_CURRENT	0x1f6	/* 101dhhhh , d=drive, hhhh=head */
#define HD_STATUS	0x1f7	/* see status-bits */
#define HD_COMMAND  HD_STATUS	/* same address, read=status, write=cmd */

#define HD_CMD		0x3f6

/* Bits of HD_ERROR */
#define HD_ERR_MARK	0x01
#define HD_ERR_TRK0	0x02
#define HD_ERR_ABRT	0x04
#define HD_ERR_ID	0x10
#define HD_ERR_ECC	0x40
#define	HD_ERR_BBD	0x80

/* Bits of HD_STATUS */
#define HD_STAT_ERR	0x01
#define HD_STAT_IDX	0x02
#define HD_STAT_ECC	0x04
#define HD_STAT_DRQ	0x08
#define HD_STAT_SEEK	0x10
#define HD_STAT_WRERR	0x20
#define HD_STAT_DRDY	0x40
#define HD_STAT_BUSY	0x80

/* Values for HD_COMMAND */
#define WIN_RESTORE	0x10
#define WIN_READ	0x20
#define WIN_WRITE	0x30
#define WIN_VERIFY	0x40
#define WIN_FORMAT	0x50
#define WIN_INIT	0x60
#define WIN_SEEK 	0x70
#define WIN_DIAGNOSE	0x90
#define WIN_SPECIFY	0x91
#define WIN_IDENTIFY	0xEC

struct partition {
	unsigned char boot_ind;
	unsigned char head;
	unsigned char sector;
	unsigned char cyl;
	unsigned char sys_ind;
	unsigned char end_head;
	unsigned char end_sector;
	unsigned char end_cyl;
	unsigned int  start_sect;
	unsigned int  nr_sects;
};

struct hd_ctl{
	unsigned long LBA;
	unsigned char busy;
	unsigned char cmd;
	struct buffer *buf;
	struct task  *wait;
};

#endif
