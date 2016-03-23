/*
 *    include/firekylin/sched.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _SCHED_H
#define _SCHED_H

#include <sys/types.h>
#include <signal.h>

struct tss_struct {
	unsigned long link;
	unsigned long esp0;
	unsigned long ss0;
	unsigned long esp1;
	unsigned long ss1;
	unsigned long esp2;
	unsigned long ss2;
	unsigned long cr3;
	unsigned long eip;
	unsigned long eflags;
	unsigned long eax;
	unsigned long ecx;
	unsigned long edx;
	unsigned long ebx;
	unsigned long esp;
	unsigned long ebp;
	unsigned long esi;
	unsigned long edi;
	unsigned long es;
	unsigned long cs;
	unsigned long ss;
	unsigned long ds;
	unsigned long fs;
	unsigned long gs;
	unsigned long ldt;
	unsigned short trap;
	unsigned short iobase;
};

struct timer{
	clock_t t_time;
	void (*t_fun)(void);
	struct timer *t_next;
};

#define NR_OPEN		32
struct task {
	long    kesp; 		/* kernel stack_end. 	*/
	pid_t   pid; 		/* process id.  	*/
	pid_t   grp; 		/* process group id. 	*/
	pid_t   sid; 		/* session id.  	*/
	uid_t   uid; 		/* user id. 		*/
	gid_t   gid; 		/* user group id. 	*/
	clock_t start;		/* start time. 		*/
	clock_t stime; 		/* system time. 	*/
	clock_t utime; 		/* user   time. 	*/
	clock_t cstime; 	/* child system time. 	*/
	clock_t cutime; 	/* child user   time. 	*/
	clock_t alarm; 		/* alarm. 		*/
	long    state; 		/* process state. 	*/
	long    count; 		/* have not used time. 	*/
	long    priority; 	/* process priority.	*/
	long    pdtr; 		/* cr3 			*/
	long    sbrk; 		/* heap end. 		*/
	long    stack; 		/* stack end. 		*/
	long 	status; 	/* exit status. 	*/
	sigset_t sig_mask;
	sigset_t sig_signal; 	/* Receive signal.	*/
	struct sigaction sigtable[32];
	unsigned int tty; 	/* use which tty. 	*/
	struct inode *pwd; 	/* current dir inode. 	*/
	struct file *file[NR_OPEN]; /* file open. 	*/
	struct timer timer;
	struct task *parent;
	struct task *next;
};

/* Values of task->state */
#define TASK_RUN	1
#define TASK_WAIT	2
#define TASK_EXIT	3
#define TASK_WAIT_CHLD	4

#define NR_TASK		16

#define CURRENT_TASK() 			\
    (struct task*)({ 			\
	unsigned long __res; 		\
	asm volatile(			\
            "movl %%esp,%%eax;"		\
            "andl $0xfffff000,%%eax"	\
            :"=a"(__res)); 		\
	__res; 				\
    })

#define __switch_to(n)				\
    ({	asm volatile (				\
		"pushl $__ret_switch;"		\
		"pushf;"			\
		"pushl %%eax;"			\
		"pushl %%ebx;"			\
		"pushl %%ecx;"			\
		"pushl %%edx;"			\
		"pushl %%edi;"			\
		"pushl %%esi;"			\
		"pushl %%ebp;"			\
		"movl  %%esp,%%eax;"		\
		"andl  $0xfffff000,%%eax;"	\
		"movl  %%esp,(%%eax);"		\
		"movl  %%ecx,%%esp;"		\
		"popl  %%ebp;"			\
		"popl  %%esi;"			\
		"popl  %%edi;"			\
		"popl  %%edx;"			\
		"popl  %%ecx;"			\
		"popl  %%ebx;"			\
		"popl  %%eax;"			\
		"popf;"				\
		"ret;"				\
		"__ret_switch:  ;"		\
		::"c"(task_table[n]->kesp));	\
    })

extern struct task * task_table[];

extern time_t start_time;
extern clock_t clock;

void sched(void);
void sleep_on(struct task **p);
void wake_up(struct task **p);

#endif
