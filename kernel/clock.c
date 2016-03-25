/*
 *    kernel/clock.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <firekylin/trap.h>
#include <time.h>
#include <errno.h>

#define LATCH		(1193180/CLOCKS_PER_SEC)

time_t start_time;
clock_t clock;

static struct timer *timer_list;

void rm_timer(struct timer *timer)
{
	struct timer *timer_tmp;

	if(timer_list == timer){
		timer_list=timer->t_next;
		return ;
	}
	timer_tmp=timer_list;
	while(timer_tmp){
		if(timer_tmp->t_next==timer){
			timer_tmp->t_next=timer->t_next;
			return ;
		}
		timer_tmp=timer_tmp->t_next;
	}
}

void add_timer(long much, void (*fn)(void))
{
	struct task *current=CURRENT_TASK();
	struct timer *timer=&current->timer;
	struct timer *timer_tmp;

	if(timer->t_time)
		rm_timer(timer);

	timer->t_time=clock+much;
	timer->t_fun=fn;

	if(!timer_list){
		timer_list=timer;
		timer->t_next=NULL;
		return ;
	}

	if(timer_list->t_time<timer->t_time){
		timer->t_next=timer_list;
		timer_list=timer;
		return ;
	}

	timer_tmp=timer_list;
	while(timer_tmp->t_next && timer->t_next->t_time <timer->t_time)
		timer_tmp=timer_tmp->t_next;

	timer->t_next=timer_tmp->t_next;
	timer_tmp->t_next=timer;
}

static void do_clock(struct trapframe *tf)
{
	struct task *current=CURRENT_TASK();

	outb(0x20, 0x20);
	clock++;

	if(tf->cs&3){
		current->utime++;
	}else
		current->stime++;

	if(timer_list && timer_list->t_time <=clock){
		void (*fn)(void);
		timer_list->t_time=0;
		fn=timer_list->t_fun;
		timer_list=timer_list->t_next;
		if(fn){
			(fn)();
		}
	}
	if (--current->count < 0) {
		current->count = 0;
		sched();
	}
}

void clock_init()
{
	outb(0x43, 0x36);
	outb(0x40, LATCH&&0xff);
	outb(0x40, LATCH>>8);
	set_trap_handle(0x20, &do_clock);
	outb(0x21, inb(0x21)&~1);
}

int sys_getime(long *res)
{
	time_t i = current_time();

	if (res) {
		*res = i;
	}
	return i;
}

int sys_setime(long *p)
{
	if ((CURRENT_TASK() )->uid != 0)
		return -EPERM;

	return start_time = *p - clock / CLOCKS_PER_SEC;
}
