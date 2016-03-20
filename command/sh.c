/*
 *    command/sh.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>

#define MAX_ARG 64

char *ps1 = "[firkylin~]#";
int size;

int get_cmd(char *buf)
{
	size = read(0, buf, 100);
	buf[size] = 0;
	return size;
}

void parcmd(char *buf, char **argv)
{
	for (int i = 0; i < MAX_ARG; i++) {
		while (*buf == ' ')
			buf++;
		if (*buf == 0) {
			argv[i] = NULL;
			break;
		}

		argv[i] = buf;

		while (*buf && *buf != ' ')
			buf++;

		if (*buf != 0)
			*buf++ = 0;
	}
}

void execcmd(char **argv)
{
	int pid;
	if (strcmp(argv[0], "cd") == 0) {
		chdir(argv[1]);
		return ;
	}
	pid = fork();
	if (pid) {
		waitpid(pid, NULL, 0);
	} else {
		_exit(execve(argv[0], argv, NULL));
	}
}

int main(void)
{
	int pid;
	char *argv[MAX_ARG];
	char buf[100];

	while (1) {
		printf("%s", ps1);
		if (get_cmd(buf)) {
			parcmd(buf, argv);
			execcmd(argv);
		}
	}
}
