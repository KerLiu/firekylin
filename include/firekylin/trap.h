/*
 *    include/firekylin/trap.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _TRAP_H
#define _TRAP_H

struct trapframe {
	unsigned long eax;
	unsigned long ebx;
	unsigned long ecx;
	unsigned long edx;
	unsigned long edi;
	unsigned long esi;
	unsigned long ebp;
	unsigned long ds;
	unsigned long es;
	unsigned long nr;
	unsigned long err;
	unsigned long eip;
	unsigned long cs;
	unsigned long eflags;
	unsigned long esp;
	unsigned long ss;
};

#define T_DIVIDE         0      /* divide error				*/
#define T_DEBUG          1      /* debug exception			*/
#define T_NMI            2      /* non-maskable interrupt		*/
#define T_BRKPT          3      /* breakpoint				*/
#define T_OFLOW          4      /* overflow				*/
#define T_BOUND          5      /* bounds check				*/
#define T_ILLOP          6      /* illegal opcode 			*/
#define T_DEVICE         7      /* device not available			*/
#define T_DBLFLT         8      /* double fault				*/
#define T_COPROC         9      /* reserved (not used since 486)	*/
#define T_TSS           10      /* invalid task switch segment		*/
#define T_SEGNP         11      /* segment not present			*/
#define T_STACK         12      /* stack exception			*/
#define T_GPFLT         13      /* general protection fault		*/
#define T_PGFLT         14      /* page fault				*/
#define T_RES           15      /* reserved				*/
#define T_FPERR         16      /* floating point error			*/
#define T_ALIGN         17      /* aligment check			*/
#define T_MCHK          18      /* machine check			*/
#define T_SIMDERR       19      /* SIMD floating point error		*/

#define T_IRQ           32

#define IRQ_TIMER        (T_IRQ+0)
#define IRQ_KBD          (T_IRQ+1)
#define IRQ_COM1         (T_IRQ+4)
#define IRQ_IDE         (T_IRQ+14)

extern void set_trap_handle(int index, void (*fn)(struct trapframe *tf));

#endif
