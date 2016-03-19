/*
 *    kernel/main.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/sched.h>
#include <firekylin/driver.h>
#include <firekylin/fs.h>

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

void start(void)
{
	memset(__bss_start__, 0, (__bss_end__-__bss_start__));

	arch_init();
	tty_init();
	hd_init();
	mm_init();
	buffer_init();
	sched_init();
	clock_init();

	if (sys_fork()) {
		asm("__hlt:hlt ; jmp __hlt");
	}

	mount_root();
	sys_exec("/sys/bin/init", NULL, NULL);
	panic("Can't find init");
}
