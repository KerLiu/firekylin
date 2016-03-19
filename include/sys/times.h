/*
 *    include/sys/times.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 *
 */

#ifndef _SYS_TIMES_H
#define _SYS_TIMES_H

#include <sys/types.h>

struct tms {
	clock_t tms_utime; 	/* User CPU time.                     */
	clock_t tms_stime; 	/* System CPU time.                   */
	clock_t tms_cutime; 	/* User CPU time of child processes.  */
	clock_t tms_cstime; 	/* System CPU time of child processes.*/
};

extern clock_t times(struct tms *tmsptr);

#endif
