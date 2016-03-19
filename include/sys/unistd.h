/*
 *    include/sys/unistd.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _SYS_UNISTD_H
#define _SYS_UNISTD_H

#include <sys/types.h>

/* Values used by access() */
#define F_OK	0	/* test file is exist      */
#define X_OK	1	/* test file is executable */
#define W_OK	2	/* test file is writable   */
#define R_OK	4	/* test file is readable   */

/* standard I/O fileno */
#define STDIN_FILENO	0	/* file descriptor of stdin  */
#define STDOUT_FILENO	1	/* file descriptor of stdout */
#define STDERR_FILENO	2	/* file descriptor of stderr */

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

extern int errno;
extern char **environ;

extern pid_t fork();
extern pid_t getpid();
extern pid_t getppid();
extern ssize_t read(int fd,char * buf,size_t size);
extern ssize_t write(int fd,char * buf,size_t size);
extern int execve(char *filename, char **argv, char **envp);
extern void _exit(int status);
extern int sbrk(int inc);
extern int dup(int fd);

#endif
