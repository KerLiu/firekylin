/*
 *    command/echo.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <sys/unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	while (*argv) {
		printf("%s\n", *argv);
		argv++;
	}
}
