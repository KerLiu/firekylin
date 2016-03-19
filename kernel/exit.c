/*
 *    kernel/exit.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <firekylin/mm.h>
#include <firekylin/fs.h>
#include <sys/types.h>
#include <errno.h>

extern int sys_close(int fd);

void do_exit(long exitcode)
{
	struct task * current=CURRENT_TASK();
	current->state = TASK_EXIT;
	current->status=exitcode;
	free_mm();
	if(current->parent->state == TASK_WAIT_CHLD){
		current->parent->state = TASK_RUN;
	}
	
	if(current->pwd)
		iput(ilock(current->pwd));
	for(int i=0;i<NR_OPEN;i++)
		if(current->file[i])
			sys_close(i);
	sched();
}

void sys_exit(long status)
{
	do_exit((status << 8) & 0xff00);
}

int sys_pwait(pid_t pid, long *status,int options)
{
	int ret;
	struct task *current=CURRENT_TASK();

	while (1) {
		for (int i = 0; i < NR_TASK; i++) {
			if (!task_table[i]
				|| task_table[i]->state != TASK_EXIT
				|| task_table[i]->parent->pid!= current->pid) {
				continue;
			}
			ret = task_table[i]->pid;
			if(status){
				*status=task_table[i]->status;
			}
			put_page(__pa((long )task_table[i]));
			task_table[i]=NULL;
			return ret;
		}
		current->state=TASK_WAIT_CHLD;
		sched();
	}
	return pid;
}

int sys_ptrace()
{
	return -ERROR;
}
