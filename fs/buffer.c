/*
 *    fs/buffer.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>
#include <sys/types.h>
#include <errno.h>

#define NR_BUFFER	64

static unsigned long buffer_busy=0;
static struct task * buffer_wait=NULL;
struct buffer buffer_table[NR_BUFFER];
struct buffer * free_list_head;

static inline void lock_buffer_table(void)
{
	irq_lock();
	while (buffer_busy){
		sleep_on(&buffer_wait);
	}
	buffer_busy = 1;
	irq_unlock();
}

static inline void unlock_buffer_table(void)
{
	irq_lock();
	buffer_busy = 0;
	wake_up(&buffer_wait);
	irq_unlock();
}

static inline void lock_buffer(struct buffer *buf)
{
	irq_lock();
	while(buf->b_flag&B_BUSY)
		sleep_on(&buf->b_wait);
	buf->b_flag|=B_BUSY;
	irq_unlock();
}

static inline void unlock_buffer(struct buffer *buf)
{
	irq_lock();
	buf->b_flag&=~B_BUSY;
	wake_up(&buf->b_wait);
	irq_unlock();
}

struct buffer * getblk(dev_t dev, int block)
{
	struct buffer *buf;

	lock_buffer_table();
	buf = buffer_table;
	while(buf < buffer_table + NR_BUFFER) {
		if ((buf->b_dev == dev) && (buf->b_block == block)) {
			buf->b_count++;
			unlock_buffer_table();
			lock_buffer(buf);
			return buf;
		}
		buf++;
	}

	if(!free_list_head){
		panic("No availabel buffer");
	}

	if(free_list_head==free_list_head->b_free_next){
		buf=free_list_head;
		free_list_head=NULL;
	}else{
		buf=free_list_head;
		free_list_head=free_list_head->b_free_next;
		buf->b_free_prev->b_free_next=buf->b_free_next;
		buf->b_free_next->b_free_prev=buf->b_free_prev;
	}
	buf->b_count++;
	unlock_buffer_table();
	lock_buffer(buf);
	if(buf->b_flag&B_DIRTY)
			write_block(buf);
	buf->b_dev = dev;
	buf->b_block = block;
	buf->b_flag&=~B_VALID;
	return buf;
}

void brelse(struct buffer *buf)
{
	lock_buffer_table();
	if (--buf->b_count < 0) 
		panic("put_buffer:buffer_count <0");

	if(buf->b_count==0){
		if(!free_list_head)
			free_list_head=buf->b_free_next=buf->b_free_prev=buf;
		else{
			buf->b_free_next=free_list_head;
			buf->b_free_prev=free_list_head->b_free_prev;
			buf->b_free_prev->b_free_next=buf;
			free_list_head->b_free_prev=buf;
		}
		wake_up(&buffer_wait);
	}
	unlock_buffer_table();
	unlock_buffer(buf);
}

struct buffer * bread(dev_t dev,long block)
{

	struct buffer *buf=getblk(dev,block);

	if(buf==NULL)
		panic("read_buffer:get_buffer return NULL");

	if(!(buf->b_flag&B_VALID)){
		read_block(buf);
		lock_buffer(buf);
	}
	return buf;
}

int sys_sync()
{
	extern void sync_inode();
	struct buffer *bh;

	sync_inode();
	for (bh = buffer_table; bh < buffer_table + NR_BUFFER; bh++) {
		lock_buffer(bh);
		if (bh->b_flag & B_DIRTY) {
			write_block(bh);
			bh->b_flag &= ~B_DIRTY;
		}
		unlock_buffer(bh);
	}
	return 0;
}

void buffer_init(void)
{
	struct buffer *buf;

	for (buf = buffer_table; buf < buffer_table + NR_BUFFER;buf++) {
		buf->b_dev=0;
		buf->b_block=0;
		buf->b_flag=0;
		buf->b_count=0;
		buf->b_hash_prev=NULL;
		buf->b_hash_next=NULL;
		buf->b_free_prev=buf-1;
		buf->b_free_next=buf+1;
		buf->b_wait=NULL;
	}

	buffer_table[0].b_free_prev=&buffer_table[NR_BUFFER-1];
	buffer_table[NR_BUFFER-1].b_free_next=buffer_table;
	free_list_head=buffer_table;
}
