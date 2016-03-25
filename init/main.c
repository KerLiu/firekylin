/*
 *    kernel/main.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>
#include <time.h>

#define CMOSREAD(value,index)			\
    asm("mov $0x70,%%dx\t\n"  			\
        "out %%al,%%dx\t\n"			\
        "inc %%dx\t\n"				\
        "in  %%dx,%%al\t\n"			\
        :"=a"(value)				\
        :"a"(index)				\
        :"dx")

#define BCD_BIN(c)	(c=c/16*10+c%16)

extern char __bss_start__[];
extern char __bss_end__[];

extern void arch_init();
extern void tty_init();
extern void hd_init(void);
extern void mm_init();
extern void buffer_init();
extern void sched_init();
extern void clock_init();
extern int  sys_fork();
extern int  sys_exec(char *filename, char **argv, char **envp);
extern void mount_root(void);

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

static void time_init()
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
}

void start(void)
{
	memset(__bss_start__, 0, (__bss_end__-__bss_start__));

	arch_init();
	tty_init();
	time_init();
	hd_init();
	mm_init();
	buffer_init();
	sched_init();
	clock_init();

	if (sys_fork()) {
		asm("__hlt:hlt ; jmp __hlt");
	}

	mount_root();

	sys_exec("/bin/init", NULL, NULL);
	panic("Can't find init");
}
