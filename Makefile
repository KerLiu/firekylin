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
     fs/minix/super.o fs/minix/inode.o fs/minix/bitmap.o \
     fs/minix/namei.o fs/minix/file.o fs/minix/operation.o 
       
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
	make -C command

install:
	make install -C command
clean:
	-del boot.bin kernel.bin kernel.map build.exe System.img
	-del driver\block\*.o driver\char\*.o  mm\*.o
	-del init\*.o kernel\*.o driver\*.o fs\*.o fs\minix\*.o 
	-del libc\*.a libc\*.o libc\string\*.o libc\unistd\*.o libc\stdlib\*.o
	-del libc\ctype\*.o libc\crt\*.o libc\stdio\*.o
	-del libc\signal\*.o libc\dirent\*.o
	make clean -C command
	
run: System.img
	bochs -q -f bochsrc

dbg: System.img
	bochsdbg -q -f bochsrc

count:
	@echo dirs: $(shell ls -lR |grep ^d |wc -l)
	@echo files: $(shell ls -lR |grep ^- |wc -l)
	@echo lines: $(shell find . -name *.[chs] |xargs grep -v ^$$ |wc -l)
