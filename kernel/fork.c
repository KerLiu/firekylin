/*
 *    kernel/fork.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <firekylin/mm.h>
#include <firekylin/fs.h>
#include <sys/types.h>
#include <errno.h>

static pid_t last_pid = 0;

int sys_fork(long unuesd)
{
	struct task * task;
	struct task * current;
	unsigned long *stack;

	task = (struct task*) __va(get_page());
	current=CURRENT_TASK();
	memcpy(task, current , 4096);
	task->pdtr = copy_mm();
	task->pid = ++last_pid;
	task->parent = current;
	

	task->utime=0;
	task->stime=0;
	task->cutime=0;
	task->cstime=0;
	task->start=current_time();
	stack = (unsigned long *) ((((long) &unuesd) & 0xfff) | (long) task);
	--stack;		// ret
	*(--stack) = 0x13200;	// popf
	*(--stack) = 0;		// eax
	*(--stack) = 0;		// ebx
	*(--stack) = 0;		// ecx
	*(--stack) = 0;		// edx
	*(--stack) = 0;		// edi
	*(--stack) = 0;		// esi
	*(--stack) = 0;		// ebp

	task->kesp = (unsigned long) stack;
	task->state = TASK_RUN;

	if(current->pwd)
		current->pwd->i_count++;
	for(int i=0;i<NR_OPEN;i++){
		if(current->file[i]){
			current->file[i]->f_count++;
		}
	}

	for (int i = 0; i < NR_TASK; i++) {
		if (task_table[i])
			continue;
		task_table[i] = task;
		return task->pid;
	}
}
