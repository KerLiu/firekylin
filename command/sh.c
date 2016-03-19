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

char *ps1 = "[firkylin~]#";
char cmd[100];
int size;

int get_cmd()
{
	size=read(0,cmd,100);
	cmd[size]=0;
	return size;
}

int main(void)
{
	int pid;

	while (1) {
		printf("%s", ps1);
		if (get_cmd()) {
			pid = fork();
			if (pid) {
				waitpid(pid, NULL, 0);
			} else {
				execve(cmd, NULL, NULL);
				_exit(0);
			}
		}
		printf("\n");
	}
}
