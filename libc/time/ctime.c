/*
 *	libc/time/ctime.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <time.h>

char *ctime(const time_t *tp)
{
        return(asctime(localtime(tp)));
}