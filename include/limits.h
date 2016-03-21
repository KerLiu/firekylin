/*
 *    include/limits.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _LIMITS_H
#define _LIMITS_H

/* definitions of char */
#define CHAR_BIT	   8
#define CHAR_MAX	 127
#define CHAR_MIN	-128
#define SCHAE_MAX	 127
#define SCHAR_MIN	-128
#define UCHAR_MAX	 255
#define UCHAR_MIN	   0

/* definitions of short */
#define SHORT_BIT	    16
#define SHORT_MIN	-32768
#define SHORT_MAX	 32767
#define USHORT_MIN	     0
#define USHORT_MAX	0xFFFF

/* definitions of int */
#define INT_BIT		         32
#define INT_MIN		-2147483648
#define INT_MAX		 2147483647
#define UINT_MIN	          0
#define UINT_MAX	 0xFFFFFFFF

/* Definitions of long */
#define LONG_BIT	          32
#define LONG_MIN	-2147483648L
#define LONG_MAX	 2147483647L
#define ULONG_MIN	          0L
#define ULONG_MAX	 0xFFFFFFFFL

#define PATH_MAX	512
#endif
