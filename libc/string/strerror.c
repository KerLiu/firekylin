/*
 *	libc/string/strerror.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

static char *err_str[]={
	"unknown error",
	"generic error",
	"operation not permitted",
	"no such file or directory",
	"interrupted function call",
	"input/output error",
	"arg list too long",
	"is a directory",
	"invalid argument",
	"permission denied",
	"bad file descriptor",
	"not a directory",
	"not enough space",
	"resource temporarily unavailable",
	"exec format error",
	"no such device",
	"too many open files"
};

char *strerror(int errno)
{
	if(errno<0 || errno>16){
		return err_str[0];
	}
	return err_str[errno];
}
