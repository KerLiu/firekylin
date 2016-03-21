/*
 *    kernel/signal.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <signal.h>
#include <errno.h>

void do_signal(struct trapframe *tf)
{

}

int sys_sigact()
{
	return -ERROR;
}

int sys_sigsend()
{
	return -ERROR;
}

int sys_sigmask(int how,sigset_t *set ,sigset_t *oset)
{
	struct task *current;

	current=CURRENT_TASK();
	if(oset)
		*oset=current->sig_mask;
	if(set){
		switch(how){
			case SIG_BLOCK:
				current->sig_mask=current->sig_mask | *set;
				//current->sig_mask&=(~(1<<(SIGKILL-1))|1<<(SIGSTOP-1));
				break;
			case SIG_UNBLOCK:
				current->sig_mask=current->sig_mask &(~(*set));
				break;
			case SIG_SETMASK:
				current->sig_mask=(*set);
		}
	}
	return current->sig_mask;
}

int sys_sigwait()
{
	return -ERROR;
}

int sys_alarm()
{
	return -ERROR;
}
