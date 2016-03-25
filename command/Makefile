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
LIB=../libc/libc.a

commands:
	$(CC) $(CFLAGS) -o init.o init.c
	$(CC) $(CFLAGS) -o sh.o sh.c
	$(CC) $(CFLAGS) -o ls.o ls.c
	$(CC) $(CFLAGS) -o echo.o echo.c
	$(CC) $(CFLAGS) -o cat.o cat.c
	$(CC) $(CFLAGS) -o sync.o sync.c
	$(CC) $(CFLAGS) -o mkdir.o mkdir.c
	$(CC) $(CFLAGS) -o touch.o touch.c
	$(CC) $(CFLAGS) -o cp.o cp.c
	$(CC) $(CFLAGS) -o ll.o ll.c

install:
	$(LD) -o init init.o  $(LIB)
	$(LD) -o sh sh.o  $(LIB)
	$(LD) -o ls ls.o  $(LIB)
	$(LD) -o echo echo.o  $(LIB)
	$(LD) -o cat cat.o  $(LIB)
	$(LD) -o sync sync.o  $(LIB)
	$(LD) -o mkdir mkdir.o  $(LIB)
	$(LD) -o touch touch.o  $(LIB)
	$(LD) -o cp cp.o  $(LIB)
	$(LD) -o ll ll.o  $(LIB)
	mkdir tmpdir
	sudo mount -t minix ../1.44.img -o loop tmpdir
	cp   init tmpdir/bin/init
	cp   sh tmpdir/bin/sh
	cp   ls tmpdir/bin/ls
	cp   echo tmpdir/bin/echo
	cp   cat tmpdir/bin/cat
	cp   sync tmpdir/bin/sync
	cp   mkdir tmpdir/bin/mkdir
	cp   touch tmpdir/bin/touch
	cp   cp tmpdir/bin/cp
	cp   ll tmpdir/bin/ll
	sudo umount ../1.44.img
	rmdir tmpdir

clean:
	-del init sh ls echo cat mkdir cp sync touch ll
	-del *.o