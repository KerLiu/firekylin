/*
 *    command/argv.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
	while (*argv) {
		printf("%s\n", *argv);
		argv++;
	}
	while (*envp) {
		printf("%s\n", *envp);
		envp++;
	}
}
