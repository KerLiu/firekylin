/*
 *    command/init.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <sys/wait.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv, char **envp)
{
	int pid;

	open("/sys/dev/tty1",O_READ|O_WRITE,0);
	printf("\n\nWelcome To FireKylin 0.1 !\n\n\n");
	
	while(1){
		pid=fork();
		if(pid){
			waitpid(pid,NULL,0);
		} else{
			execve("/sys/bin/sh",NULL,NULL);
			_exit(0);
		}
	}
}
