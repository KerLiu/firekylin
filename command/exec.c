/*
 *    command/exec.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <stdio.h>

#define MAX_ARG 64

void parcmd(char *buf, char **argv)
{
	int i;
	char **tmp;

	for (i = 0; i < MAX_ARG-1 ; i++) {
		while (*buf == ' ')
			buf++;
		if (*buf==0) {
			argv[i] = NULL;
			break;
		}

		argv[i] = buf;

		while (*buf && *buf != ' ')
			buf++;

		if (*buf!=0)
			*buf++ = 0;
	}
	argv[i+1]=0;
}

int main(void)
{
	char *argv[MAX_ARG];

	char *envp[] = { "HOME=/user/lxf", NULL };
	char *buf = "/sys/bin/argv hello os world";

	int size;
	int fd=open("/sys/dev/tty1", O_READ | O_WRITE, 0);
	char buf2[100];
	size=read(fd,buf2,100);
	buf2[size]=0;
	parcmd(buf2, argv);
	execve(argv[0], argv, envp);
}
