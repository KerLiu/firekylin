/*
 *    kernel/sched.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <firekylin/mm.h>
#include <sys/types.h>
#include <sys/times.h>
#include <errno.h>

static struct tss_struct tss;

struct task * task_table[NR_TASK];

static void init_tss(void)
{
	extern struct gdt_desc {
		unsigned long low;
		unsigned long high;
	} gdt_table[6];

	long addr=(long)&tss;
	
	tss.ss0=0x10;
	gdt_table[5].low=((addr<<16)&0xffff0000) |104;
	gdt_table[5].high=(addr&0xff000000) | 0xe900 | ((addr>>16)&0xff);

	asm("ltr %%ax"::"a"(0x28));
}

void sched_init(void)
{
	struct task *current = CURRENT_TASK();
	init_tss();
	memset(current, 0, sizeof(struct task));
	current->count = current->priority = 20;
	current->state = TASK_RUN;
	task_table[0] = current;
}

void sched(void)
{
	int i, c = -1, n = 0;

	while (1) {
		for (i = NR_TASK - 1; i > 0; i--) {
			if (!task_table[i] || task_table[i]->state != TASK_RUN)
				continue;
			if (task_table[i]->count > c)
				c = task_table[i]->count,n = i;
		}
		
		if (c) {
			if ((CURRENT_TASK() ) != task_table[n]) {
				tss.esp0=(long)task_table[n]+4096;
				asm("movl %%eax,%%cr3"
						::"a"(task_table[n]->pdtr));
				__switch_to(n);
			}
			return;
		}
		
		for (i = 1; i < NR_TASK; i++) {
			if (task_table[i]) {
				task_table[i]->count = task_table[i]->priority;
			}
		}
	}
}

void sleep_on(struct task **p)
{
	struct task *task = CURRENT_TASK();

	task->state = TASK_WAIT;
	if (p) {
		task->next = *p;
		*p = task;
	}
	sched();
}

void wake_up(struct task **p)
{
	struct task *tmp;
	if(!p)
		return ;
	while (*p) {
		tmp = *p;
		tmp->state = TASK_RUN;
		*p = tmp->next;
		tmp->next = NULL;
	}
}

pid_t sys_getpid(void)
{
	return (CURRENT_TASK() )->pid;
}

pid_t sys_setgrp(void)
{
	struct task *current=CURRENT_TASK();
	return current->grp = current->pid;
}

pid_t sys_setsid(void)
{
	struct task *task = CURRENT_TASK();

	if ((task->sid == task->pid) || (task->uid != 0))
		return -EPERM;
	task->tty = -1;
	return task->sid = task->grp = task->pid;
}

uid_t sys_getuid(void)
{
	return (CURRENT_TASK() )->uid;
}

uid_t sys_setuid(uid_t uid)
{
	if ((CURRENT_TASK() )->uid != 0)
		return -EPERM;
	return (CURRENT_TASK() )->uid = uid;
}

gid_t sys_getgid(void)
{
	return (CURRENT_TASK() )->gid;
}

gid_t sys_setgid(gid_t gid)
{
	if ((CURRENT_TASK() )->uid != 0)
		return -EPERM;
	return (CURRENT_TASK() )->gid = gid;
}

/*
 * Note: Align to 16 Bytes
 */
int sys_sbrk(unsigned int inc)
{
	unsigned int res;
	unsigned long addr;
	struct task *current;
	
	if (inc < 0)
		return -1;
	
	current=CURRENT_TASK();
	res =current->sbrk;
	inc=(inc+0xf)&0xfffffff0;
	current->sbrk += inc;
	
	addr=(addr+0xfff)&0xfffff000;
	for(;addr<=current->sbrk; addr+=4096){
		map_page(addr,get_page(),current->pdtr);
	}
	return res;
}

int sys_times(struct tms *tmsptr)
{
	struct task *task = CURRENT_TASK();
	if (tmsptr) {
		tmsptr->tms_utime = task->utime;
		tmsptr->tms_stime = task->stime;
		tmsptr->tms_cutime = task->cutime;
		tmsptr->tms_cstime = task->cstime;
	}
	return task->start;
}
