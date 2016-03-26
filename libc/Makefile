#
#    libc/Makefile
#
#    Copyright (C) 2016 ximo<ximoos@foxmail.com>
#

AS=nasm
CC=tcc

SFLAGS=-f elf
CFLAGS=-c -Wall -nostdinc -nostdlib -I ../include

OBJS= crt/crt.o  \
      stdlib/abs.o stdlib/labs.o stdlib/malloc.o stdlib/getenv.o \
      string/memcpy.o string/memset.o string/strcat.o string/strcpy.o \
      string/strlen.o string/strncpy.o string/strcmp.o string/vsprintf.o \
      string/strerror.o unistd/fork.o unistd/getpid.o  string/strncmp.o \
      unistd/getuid.o unistd/setuid.o unistd/setsid.o  unistd/create.o \
      unistd/errno.o  unistd/open.o unistd/read.o unistd/stat.o unistd/link.o \
      unistd/execve.o unistd/_exit.o unistd/waitpid.o unistd/sbrk.o \
      stdio/printf.o  unistd/write.o unistd/dup.o unistd/fstat.o \
      unistd/chdir.o unistd/close.o  unistd/sync.o unistd/mkdir.o \
      ctype/__ctype.o ctype/isalnum.o ctype/isalpha.o ctype/iscntrl.o \
      ctype/isdigit.o ctype/islower.o ctype/isprint.o ctype/ispuntc.o \
      ctype/isupper.o ctype/isxdigit.o ctype/isspace.o ctype/isgraph.o \
      ctype/tolower.o ctype/toupper.o  \
      signal/sigaction.o   signal/sigaddset.o  signal/sigdelset.o   \
      signal/sigemptyset.o signal/sigfillset.o signal/sigismember.o \
      signal/sigprocmask.o  \
      dirent/opendir.o dirent/readdir.o dirent/rewinddir.o dirent/closedir.o
      

.s.o:
	@echo         AS	$<
	@$(AS) $(SFLAGS) -o $@ $<
.c.o:
	@echo         CC	$<
	@$(CC) $(CFLAGS) -o $@ $<

libc.a: $(OBJS)
	@echo         AR       OBJS
	@ar rcs   libc.a $(OBJS) 

clean: 
	-del libc.a
	-del stdlib\*.o string\*.o unistd\*.o stdio\*.o ctype\*.o 
	-del crt\*.o stdio\*.o
