/*
 *    kernel/memory.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/trap.h>
#include <firekylin/sched.h>
#include <firekylin/mm.h>
#include <firekylin/fs.h>

#define MAX_NR_PAGES	(64*1024*1024/4096)

static unsigned int memsize = 0;
static unsigned int NR_PAGE = 0;

struct page_struct page_table[MAX_NR_PAGES];

long get_page(void)
{
	for (unsigned int i = 0; i < NR_PAGE; i++) {
		if (!page_table[i].count) {
			page_table[i].count++;
			return (i << 12) + 0x400000;
		}
	}
	panic("out of memory");
	return 0L;
}

/*void test_mem(void)
{
	for (unsigned int i = 0; i < NR_PAGE; i++) {
		if (page_table[i].count > 1) {
			printk("warn: mem error %x,count %d",
					(i << 12) + 0xC0400000,
					page_table[i].count++);
		}
	}
}

void cal_mem(void)
{
	int count=0;
	for (unsigned int i = 0; i < NR_PAGE; i++) {
		if (page_table[i].count) {
			count++;
		}
	}
	printk("mem_info:use  %x,free %x\n",count,NR_PAGE-count);
}*/

void put_page(long addr)
{
	long tmp;

	if (addr & 0xfff)
		panic("Try to free Not align 4kB page :%x", addr);
	if (addr < 0x400000)
		panic("Try to free kernel page:%x", addr);

	tmp = (addr - 0x400000) >> 12;
	if (page_table[tmp].count != 1) {
		panic("Try to err page:%x", addr);
	}
	page_table[tmp].count--;
}

void map_page(long va, long pa, long pdtr)
{
	unsigned long *p;
	unsigned long i;
	unsigned long tmp;

	p = (unsigned long*) __va(pdtr);

	if (!(p[va >> 22 & 0x3ff] & 1)) { //PTE not exist.
		tmp = get_page();
		memset(__va(tmp), 0, 4096);
		p[va >> 22 & 0x3ff] = tmp + 7;
	}

	i = p[va >> 22 & 0x3ff] & 0xfffff000;
	p = (unsigned long *) __va(i);
	p[va >> 12 & 0x3ff] = pa + 7;
}

long unmap_page(long line, long pdtr)
{
	unsigned long *p;
	unsigned long i;

	p = (unsigned long*) __va(pdtr);
	i = p[line >> 22 & 0x3ff] & 0xfffff000;
	p = (unsigned long *) __va(i);
	i = p[line >> 12 & 0x3ff] & 0xfffff000;
	return i;
}

void do_page_fault(struct trapframe *tf)
{
	unsigned long cr2;
	struct task *current;
	static int count;
	count++;
	if (count == 20) {
		printk("Too Much Page Fault");
		irq_enable();
		while(1){
		}
	}

	asm("movl %%cr2,%%eax":"=a"(cr2));
	current = CURRENT_TASK();
	map_page(cr2 & 0xfffff000, get_page(), current->pdtr);
	current->stack = cr2 & 0xfffff000;
	//printk("NOTE:current->stack=%x\n", current->stack);
}

long copy_mm(void)
{
	long addr, tmp_addr;
	long ret = get_page();
	struct task *current = CURRENT_TASK();

	memset(__va(ret), 0, 3072);
	memcpy(__va(ret)+3072, __va(current->pdtr) + 3072, 1024);

	for (addr = current->stack; addr < current->sbrk; addr += 4096) {
		tmp_addr = get_page();
		memcpy(__va(tmp_addr), addr, 4096);
		map_page(addr, tmp_addr, ret);
	}
	return ret;
}

void alloc_mm(long pdtr, long addr, long size)
{
	long start = addr & 0xfffff000;
	long end = addr + (size + 0xfff) & 0xfffff000;
	for (int i = start; i < end; i += 4096) {
		map_page(i, get_page(), pdtr);
	}
}

/*
 *  Note : just free the page, not pdt ,pte . will lost some page.
 */
void free_mm(void)
{
	struct task *current = CURRENT_TASK();
	for (long addr = current->stack; addr < current->sbrk; addr += 4096) {
		put_page(unmap_page(addr, current->pdtr));
	}
	memset(__va(current->pdtr), 0, 3072);
}

void mm_init(void)
{
	unsigned long i, *p;

	memsize = ((*(int*) 0xc0002000) & 0xffff) * 1024 + 0x100000;
	NR_PAGE = min((memsize >> 12), MAX_NR_PAGES);

	p = (unsigned long*) 0xC0000000;
	*p = 0;
	for (i = 0; i < memsize >> 22; i++) {
		*(p + i + 0x300) = 0x100007 + 0x1000 * i;
	}

	p = (unsigned long*) 0xC0100000;
	for (i = 0; i < memsize >> 12; i++) {
		*(p + i) = 7 + 0x1000 * i;
	}

	set_trap_handle(14, do_page_fault);
}
