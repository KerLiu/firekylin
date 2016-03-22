/*
 *	libc/time/asctime.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <time.h>

static const char *day[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

static const char *month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
		"Aug", "Sep", "Oct", "Nov", "Dec" };

char * asctime(const struct tm * time)
{
	static char buf[40];

	if (time == (struct tm *) NULL)
		strcpy(buf, "??? ??? ?? ??:??:?? ????\n");
	else
		sprintf(buf, "%.3s %.3s %2d %02d:%02d:%02d %04d\n",
				day[time->tm_wday], month[time->tm_mon],
				time->tm_mday, time->tm_hour, time->tm_min,
				time->tm_sec, 1900 + time->tm_year);
	return (buf);
}
