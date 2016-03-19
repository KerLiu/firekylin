#
#    firekylin/Makefile
#
#    Copyright (C) 2016 ximo<ximoos@foxmail.com>
#

AS=nasm
CC=tcc
LD=ld -m elf_i386 -T command/ldscript

SFLAGS=-f elf
CFLAGS=-c -Wall -nostdinc -nostdlib -I ./include

OBJS=init/head.o init/main.o  \
     kernel/entry.o kernel/trap.o   kernel/util.o  \
     kernel/sched.o kernel/signal.o kernel/clock.o \
     kernel/fork.o  kernel/exit.o   kernel/exec.o  \
     driver/char/console.o driver/char/keyboard.o driver/char/tty.o  \
     driver/block/hd.o driver/driver.o \
     mm/memory.o \
     fs/buffer.o fs/super.o fs/inode.o fs/read.o fs/stat.o fs/open.o \
     fs/write.o fs/fcntl.o fs/create.o fs/pipe.o fs/ioctl.o fs/close.o \
     fs/minix/bitmap.o fs/minix/inode.o 
       
.s.o:
	@echo         AS	$<
	@$(AS) $(SFLAGS) -o $@ $<
.c.o:
	@echo         CC	$<
	@$(CC) $(CFLAGS) -o $@ $<

all: System.img libc.a commands
	
System.img:boot.bin kernel.bin build.exe
	@build boot.bin kernel.bin System.img
	
boot.bin:boot/boot.s
	$(AS) -o $@  $<
	
kernel.bin: $(OBJS)
	@echo         LD      OBJS
	@ld -e _start -Ttext 0xC0010000 -Map kernel.map -o kernel.bin $(OBJS)
	@objcopy -j .text -j .data -O binary kernel.bin
	
build.exe:tools/build.c
	$(CC) -o $@  $<

libc.a:
	make -C libc

commands:
	$(CC) $(CFLAGS) -o command/sh.o command/sh.c
	$(CC) $(CFLAGS) -o command/init.o command/init.c
	$(CC) $(CFLAGS) -o command/ls.o command/ls.c
	$(CC) $(CFLAGS) -o command/t1.o command/t1.c
	$(CC) $(CFLAGS) -o command/exec.o command/exec.c
	$(CC) $(CFLAGS) -o command/argv.o command/argv.c

clean:
	-del boot.bin kernel.bin kernel.map build.exe System.img
	-del driver\block\*.o driver\char\*.o 
	-del init\*.o kernel\*.o driver\*.o fs\*.o fs\minix\*.o 
	-del libc\*.a libc\*.o libc\string\*.o libc\unistd\*.o libc\stdlib\*.o
	-del libc\ctype\*.o
	
run: System.img
	bochs -q -f bochsrc

dbg: System.img
	bochsdbg -q -f bochsrc

install:
	$(LD) -o command/sh command/sh.o  libc/libc.a
	$(LD) -o command/init command/init.o  libc/libc.a
	$(LD) -o command/ls command/ls.o  libc/libc.a
	$(LD) -o command/argv command/argv.o  libc/libc.a
	$(LD) -o command/t1 command/t1.o  libc/libc.a
	mkdir tmpdir
	sudo mount -t minix 1.44.img -o loop tmpdir
	cp   command/sh tmpdir/sys/bin/sh
	cp   command/init tmpdir/sys/bin/init
	cp   command/ls tmpdir/sys/bin/ls
	cp   command/t1 tmpdir/sys/bin/t1
	cp   command/argv tmpdir/sys/bin/argv
	sudo umount 1.44.img
	rmdir tmpdir

count:
	@echo dirs: $(shell ls -lR |grep ^d |wc -l)
	@echo files: $(shell ls -lR |grep ^- |wc -l)
	@echo lines: $(shell find . -name *.[chs] |xargs grep -v ^$$ |wc -l)
