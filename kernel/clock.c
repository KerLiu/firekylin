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

#define CMOSREAD(value,index)			\
    asm("mov $0x70,%%dx\t\n"  			\
        "out %%al,%%dx\t\n"			\
        "inc %%dx\t\n"				\
        "in  %%dx,%%al\t\n"			\
        :"=a"(value)				\
        :"a"(index)				\
        :"dx")

#define BCD_BIN(c)	(c=c/16*10+c%16)

#define LATCH		(1193180/CLOCKS_PER_SEC)

time_t start_time;
clock_t clock;

static int month[] = { 
	0, 0, 31, 
	(31 + 28), 
	(31 + 28 + 31), 
	(31 + 28 + 31 + 30),
	(31 + 28 + 31 + 30 + 31), 
	(31 + 28 + 31 + 30 + 31 + 30), 
	(31 + 28 + 31 + 30 + 31 + 30 + 31), 
	(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31), 
	(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30), 
	(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31), 
	(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
};

static time_t mktimek(int year, int mon, int day, int hour, int min, int sec)
{
	long res = 0;

	year = year % 100 + 2000;
	for (int i = 1970; i < year; i++) {
		res += 365;
		if (isleap(i))
			res++;
	}
	res += month[mon];
	if (isleap(year) && mon > 2)
		res++;
	res = res + day - 1;
	res = res * 24 + hour;
	res = res * 60 + min;
	res = res * 60 + sec;
	return res;
}
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
	int year, month, day, hour, min, sec;

	CMOSREAD(sec, 0);
	CMOSREAD(min, 2);
	CMOSREAD(hour, 4);
	CMOSREAD(day, 7);
	CMOSREAD(month, 8);
	CMOSREAD(year, 9);
	BCD_BIN(sec);
	BCD_BIN(min);
	BCD_BIN(hour);
	BCD_BIN(day);
	BCD_BIN(month);
	BCD_BIN(year);
	start_time = mktimek(year, month, day, hour, min, sec);

	outb(0x43, 0x36);
	outb(0x40, LATCH&&0xff);
	outb(0x40, LATCH>>8);
	set_trap_handle(0x20, &do_clock);
	outb(0x21, inb(0x21)&~1);
}

time_t current_time()
{
	return start_time + clock / CLOCKS_PER_SEC;
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
