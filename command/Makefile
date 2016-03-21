#
#    firekylin/Makefile
#
#    Copyright (C) 2016 ximo<ximoos@foxmail.com>
#

AS=nasm
CC=tcc
LD=ld -m elf_i386 -T ldscript

SFLAGS=-f elf
CFLAGS=-c -Wall -nostdinc -nostdlib -I ../include

commands:
	$(CC) $(CFLAGS) -o sh.o sh.c
	$(CC) $(CFLAGS) -o init.o init.c
	$(CC) $(CFLAGS) -o ls.o ls.c
	$(CC) $(CFLAGS) -o echo.o echo.c
	$(CC) $(CFLAGS) -o cat.o cat.c

install:
	$(LD) -o sh sh.o  ../libc/libc.a
	$(LD) -o init init.o  ../libc/libc.a
	$(LD) -o ls ls.o  ../libc/libc.a
	$(LD) -o echo echo.o  ../libc/libc.a
	$(LD) -o cat cat.o  ../libc/libc.a
	mkdir tmpdir
	sudo mount -t minix ../1.44.img -o loop tmpdir
	cp   sh tmpdir/sys/bin/sh
	cp   init tmpdir/sys/bin/init
	cp   ls tmpdir/sys/bin/ls
	cp   echo tmpdir/sys/bin/echo
	cp   cat tmpdir/sys/bin/cat
	sudo umount ../1.44.img
	rmdir tmpdir

clean:
	-del init sh ls echo cat
	-del init.o sh.o ls.o  echo.o cat.o 