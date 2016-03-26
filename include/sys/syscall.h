/*
 *    include/sys/syscall.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _SYS_SYSCALL_H
#define _SYS_SYSCALL_H

#define __NR_fork		0
#define __NR_exec		1
#define __NR_sbrk		2
#define __NR_pwait		3
#define __NR_exit		4
#define __NR_ptrace		5
#define __NR_times		6
#define __NR_getpid		7
#define __NR_setgrp		8
#define __NR_setsid		9
#define __NR_getuid		10
#define __NR_setuid		11
#define __NR_getgid		12
#define __NR_setgid		13
#define __NR_sigact		14
#define __NR_sigmask		15
#define __NR_sigsend		16
#define __NR_sigwait		17
#define __NR_alarm		18
#define __NR_access		19
#define __NR_open		20
#define __NR_read		21
#define __NR_write		22
#define __NR_close		23
#define __NR_fcntl		24
#define __NR_ioctl		25
#define __NR_chdir		26
#define __NR_pipe		27
#define __NR_create		28
#define __NR_link		29
#define __NR_rename		30
#define __NR_remove		31
#define __NR_mount		32
#define __NR_umount		33
#define __NR_stat		34
#define __NR_fstat		35
#define __NR_chmod		36
#define __NR_chown		37
#define __NR_utime		38
#define __NR_getime		39
#define __NR_setime		40
#define __NR_sync		41

#define __syscall0(type,name) 			\
type name(void)					\
{						\
	long res;				\
	asm("int $0x30"				\
	   :"=a"(res)				\
	   :"a"(__NR_##name));			\
	   if(res<0){				\
		errno=-res;			\
		return -1;			\
	}					\
	return (type)res;			\
}

#define __syscall1(type,name,typeb,argb) 	\
type name(typeb argb)				\
{						\
	long res;				\
	asm("int $0x30"				\
	   :"=a"(res)				\
	   :"a"(__NR_##name),"b"(argb));	\
	   if(res<0){				\
		errno=-res;			\
		return -1;			\
	}					\
	return (type)res;			\
}

#define __syscall2(type,name,typeb,argb,typec,argc) 	\
type name(typeb argb,typec argc)			\
{							\
	long res;					\
	asm("int $0x30"					\
	   :"=a"(res)					\
	   :"a"(__NR_##name),"b"(argb),"c"(argc));	\
	   if(res<0){					\
		errno=-res;				\
		return -1;				\
	}						\
	return (type)res;				\
}

#define __syscall3(type,name,typeb,argb,typec,argc,typed,argd) 	\
type name(typeb argb,typec argc,typed argd)			\
{								\
	long res;						\
	asm("int $0x30"						\
	   :"=a"(res)						\
	   :"a"(__NR_##name),"b"(argb),"c"(argc),"d"(argd));	\
	   if(res<0){						\
		errno=-res;					\
		return -1;					\
	}							\
	return (type)res;					\
}

extern int errno;

#endif
