/*
 *    include/stddef.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _STDDEF_H
#define _STDDEF_H

#ifndef __SIZE_T
#define __SIZE_T
typedef unsigned long size_t;
#endif

#ifndef __SSIZE_T
#define __SSIZE_T
typedef long ssize_t;
#endif

#ifndef __PTRDIFF_T
#define __PTRDIFF_T
typedef long prtdiff_t;
#endif

#ifndef __WCHAR_T
#define __WCHAR_T
typedef short wchar_t;
#endif

#ifndef NULL
#define NULL	((void *)0)
#endif

#define offestof(type,member) ((size_t)&((type*)0->member))

#endif
