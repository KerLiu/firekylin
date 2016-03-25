/*
 *    fs/minix/super.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include "minix_fs.h"

int minix1_read_super(struct super *super)
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
	return 0;
}
