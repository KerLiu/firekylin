/*
 *    command/exec.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <stdio.h>

#define MAX_ARG 64

void parcmd(char *buf, char **argv)
{
	int i;
	char **tmp;
	printf("%s\n",buf);
	for (i = 0; i < MAX_ARG - 1; i++) {
		argv[i] = buf;
		if (!*buf)
			break;
		while (*buf && (*buf != ' '))
			buf++;
		*buf++ = 0;
		if (!*buf)
			break;
		while (*buf == ' ')
			buf++;
	}
	argv[i+1]=0;
	tmp = argv;
	while (*tmp) {
		printf("%s\n", *tmp);
		tmp++;
	}
}


int main(void)
{
	char *argv[MAX_ARG];

	char *envp[]={
		"HOME=/user/lxf",NULL
	};
	char *buf="/sys/bin/argv hello os world";
	parcmd(buf,argv);
	execve(argv[0], argv, envp);
}
