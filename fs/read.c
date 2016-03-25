/*
 *    fs/read.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>
#include <firekylin/mm.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <elf.h>

static int read_char(dev_t dev, char *buf, off_t off, size_t size)
{
	int major = MAJOR(dev);
	if (major > DEV_CHAR_MAX || !char_table[major])
		panic("dev %x not exsit", dev);
	return char_table[major]->read(dev, buf, off, size);
}

static int read_blk(dev_t dev, char *buf, off_t off, size_t size)
{
	struct buffer *bh;
	int block, chars, left;

	left = size;
	while (left) {
		bh = bread(dev, off / 1024);
		if (!bh)
			return -EIO;
		chars = min(left, 1024 - off % 1024);
		memcpy(buf, bh->b_data+off%1024, chars);
		brelse(bh);
		buf += chars;
		off += chars;
		left -= chars;
	}
	return size - left;
}

static int read_file(struct inode *inode, char * buf, off_t off, size_t size)
{
	return inode->i_op->file_read(inode,buf,size,off,0);
}

extern int read_pipe(struct inode *inode, char *buf, size_t size);

int sys_read(int fd, char *buf, size_t size)
{
	struct file *file;
	struct inode *inode;
	int res;

	if (fd > NR_OPEN || !(file = (CURRENT_TASK() )->file[fd]))
		return -EBADF;

	if (!(file->f_mode & O_READ))
		return -EBADF;

	inode = idup(file->f_inode);

	switch (inode->i_mode & S_IFMT) {
		case S_IFREG:
		case S_IFDIR:
			if(file->f_pos+size >inode->i_size)
					size=inode->i_size-file->f_pos;
			res = read_file(inode, buf, file->f_pos, size);
			break;
		case S_IFCHR:
			res = read_char(inode->i_rdev, buf, file->f_pos,
					size);
			break;
		case S_IFBLK:
			res = read_blk(inode->i_rdev, buf, file->f_pos,
					size);
			break;
		case S_IFIFO:
			res = read_pipe(inode, buf, size);
			break;
		default:
			res = -EIO;
	}

	iput(inode);

	if (res > 0)
		file->f_pos += res;

	return res;
}

long exec_load_file(struct inode *inode, struct buffer*bh)
{
	Elf32_Ehdr *ehdr;
	Elf32_Phdr *phdr;
	long entry;
	long buf_page;
	struct task *current = CURRENT_TASK();

	buf_page = __va(get_page());
	memcpy(buf_page, bh->b_data, 1024);
	brelse(bh);

	ehdr = (Elf32_Ehdr *) buf_page;

	for (int i = 0; i < ehdr->e_phnum; i++) {
		phdr = (Elf32_Phdr *) (buf_page + ehdr->e_phoff
				+ i * ehdr->e_phentsize);
		alloc_mm(current->pdtr, phdr->p_vaddr, phdr->p_memsz);
		read_file(inode, (char*)phdr->p_vaddr, phdr->p_offset,
				phdr->p_filesz);
		current->sbrk = phdr->p_vaddr + phdr->p_memsz;
		phdr++;
	}
	current->sbrk = (current->sbrk + 0xf) & 0xfffffff0;
	entry = ehdr->e_entry;
	put_page(__pa(buf_page));
	return entry;
}
