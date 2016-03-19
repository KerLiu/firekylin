/*
 *    include/firekylin/kernel.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _KERNEL_H
#define _KERNEL_H

#include <sys/types.h>

#define irq_disable() 			\
    asm("cli");

#define irq_enable() 			\
    asm("sti");

/* Just should be used once in a function */
#define irq_lock()			\
    unsigned long  __flag;		\
    asm("pushf;popl %0;cli":"=m"(__flag))

/* Used must after irq_lock() */
#define irq_unlock()			\
    asm("pushl %0;popf"::"m"(__flag))

#define inb(port) 			\
    ({					\
	unsigned char __res;		\
	asm volatile ("in %%dx,%%al"  	\
		  : "=a"(__res) 	\
		  :"d"(port));		\
	__res;				\
    })

#define outb(port,value)   		\
    asm("out %%al,%%dx" : :"a"(value),"d"(port))

#define ins(port,buf,size)		\
    asm("rep insw" :: "D"(buf),"d"(port),"c"(size>>1))

#define outs(port,buf,size)		\
    asm("rep outsw" :: "S"(buf),"d"(port),"c"(size>>1))

#define memcpy(dst,src,size)		\
    asm("rep movsb":: "D"(dst),"S"(src),"c"(size)); 
    
#define memset(s,c,size)		\
    asm("rep stosb" ::"D"(s),"a"(c),"c"(size));   

static inline int strncmp(char *s1, char *s2, int n)
{
	while (n--) {
		if (*s1 && (*s1 == *s2)) {
			s1++;
			s2++;
		} else {
			return (*s1 - *s2);
		}
	}
	return 0;
}  

static inline char * strncpy(char *dst, char *src, size_t n)
{
	char *t = dst;
	while (n-- && (*t++ = *src++))
		;
	return dst;
}

#define max(a,b)	((a)>=(b) ? (a) : (b))
#define min(a,b)	((a)<=(b) ? (a) : (b))

extern int printk(char * fmt, ...);
extern void panic(char * fmt, ...);

extern time_t current_time();

#endif
