/*
 *	libc/stdlib/getenv.c
 *
 *	Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <stdlib.h>
#include <string.h>

char * getenv(char * name)
{
	int len = strlen(name);
	char ** env = environ;
	char * tmp;

	while (env) {
		tmp = *env;
		if (strncmp(name, tmp, len))
			continue;
		if (tmp[len] == '=')
			return tmp + len + 1;
	}
	return NULL;
}
