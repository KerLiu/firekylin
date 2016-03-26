/*
 *    fs/minix/super.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include "minix_fs.h"

struct fs_operation minix_fs_operation={
	minix1_read_super,
	NULL,
	minix1_read_inode,
	minix1_write_inode,
	minix1_look_up,
	minix1_file_read,
	minix1_file_write,
	minix1_mknod,
	minix1_rename,
	minix1_remove,
	minix1_link
};
