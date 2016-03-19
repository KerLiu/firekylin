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

int sys_sigmask()
{
	return -ERROR;
}

int sys_sigwait()
{
	return -ERROR;
}

int sys_alarm()
{
	return -ERROR;
}
