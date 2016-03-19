/*
 *    include/stdarg.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _STDARG_H
#define _STDARG_H

typedef char * va_list;

#define va_start(ap,fmt)   ap=(va_list)&fmt+sizeof(int)
#define va_arg(ap,type)	  (ap+=sizeof(int),*((type*)(ap-sizeof(int))))
#define va_copy(d,s)	  (d)=(s)
#define va_end(ap)

#endif
