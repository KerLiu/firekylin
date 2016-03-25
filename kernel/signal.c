/*
 *    kernel/signal.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <signal.h>
#include <errno.h>

extern void do_exit(long status);

void do_signal(struct trapframe *tf)
{
	struct task *current;
	sigset_t signal;
	int signr;

	current = CURRENT_TASK();

	signal = (current->sig_signal) & (~(current->sig_mask));

	if (!signal)
		return;

	for (signr = 0; signr < 31; signr++) {
		if (!(signal & (1 << signr)))
			continue;
		if (signr == SIGCHLD - 1)
			continue;
		else
			do_exit(signr + 1);

	}

	current->sig_signal &= ~signal;
}

int sys_sigact(unsigned int signr, struct sigaction *newact,
		struct sigaction *oldact)
{
	struct task *current;

	if (signr > 32 || signr == SIGKILL)
		return -EINVAL;

	current = CURRENT_TASK();

	if (oldact)
		memcpy(oldact, &(current->sigtable[signr-1]),
				sizeof(sigaction));
	if (newact)
		memcpy(&(current->sigtable[signr-1]), newact,
				sizeof(sigaction));

	return 0;
}

int sys_sigsend(pid_t pid, int signr)
{
	int res;

	struct task **p;

	for (p = task_table; p < task_table + NR_TASK; p++) {
		if ((*p)->pid == pid) {
			(*p)->sig_signal |= 1 << (signr - 1);
			if ((*p)->state == TASK_SIGWAIT) {
				(*p)->state = TASK_RUN;
				sched();
			}
			return 0;
		}
	}
	return -ERSCH;
}

int sys_sigmask(int how, sigset_t *set, sigset_t *oset)
{
	struct task *current;

	current = CURRENT_TASK();
	if (oset)
		*oset = current->sig_mask;
	if (set) {
		switch (how) {
			case SIG_BLOCK:
				current->sig_mask = current->sig_mask | *set;
				//current->sig_mask&=(~(1<<(SIGKILL-1))|1<<(SIGSTOP-1));
				break;
			case SIG_UNBLOCK:
				current->sig_mask = current->sig_mask
						& (~(*set));
				break;
			case SIG_SETMASK:
				current->sig_mask = (*set);
		}
	}
	return current->sig_mask;
}

int sys_sigwait()
{
	(CURRENT_TASK() )->state = TASK_SIGWAIT;
	sched();
	return -EINTR;
}
