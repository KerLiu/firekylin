/*
 *	libc/string/memmove.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <string.h>

void *memmove(void *s1, const void *s2, size_t n)
{
	char *p1 = (char*) s1;
	char *p2 = (char*) s2;
	
	if(!n)
		return s1;

	if(p2<=p1 &&p2+n>p1){
		p1+=n;
		p2+=n;
		while(n--){
			*--p1=*--p2;
		}
	}else{
		while(n--){
			*p1++=*p2++;
		}
	}
	
	return s1;
}
