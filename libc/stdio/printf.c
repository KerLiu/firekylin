/*
 *	libc/stdio/printf.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <stdarg.h>

extern int vsprintf(char* buf, char* fmt, va_list ap);

int myprintf(char *fmt,...)
{
	char buf[1024];
	va_list ap;
	int i;

	va_start(ap, fmt);
	i = vsprintf(buf, fmt, ap);
	write(0,buf, i);
	return i;
}
