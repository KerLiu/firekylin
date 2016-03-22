/*
 *    driver/console.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/tty.h>

static int base = 0xc00b8000;
static int orgin = 0xc00b8000;
static int cur = 0xc00b8000;
static char color = 15;

void con_init()
{
	int a;
	outb(0x3d4, 14);
	a = inb(0x3d5);
	outb(0x3d4, 15);
	cur = base + (a & 0xff) * 512 + inb(0x3d5) * 2;
}

int con_write2(struct tty_struct *tty)
{
	char ch;
	char *tmp = (char*) cur;
	int res=0;

	while (!isempty(tty->out)) {
		GETCH(tty->out,ch);
		res++;
		if (ch > 0x1f && ch < 0x7f) {
			*tmp++ = ch;
			*tmp++ = color;
		} else if (ch == '\n') {
			tmp = (char*) ((((int) tmp - base) / 160 + 1) * 160
					+ base);
		} else if (ch == '\t') {
			tmp =(char*) ((((int) tmp - base) / 16 + 1)
							* 16 + base);
		} else if(ch=='\b'){
			*--tmp=15;
			*--tmp=0x20;
		}
		if (tmp == (char*) 0xc00bff80) {
			memcpy(base, (tmp-24*2*80), 24*2*80);
			tmp = (char*) base + 24 * 2 * 80;
			orgin = base;
		}
	}
	cur = (int) tmp;
	while (cur - orgin >= 25 * 80 * 2) {
		orgin += 160;
	}

	for(int i=0;i<80;i++){
		*tmp++=0x20;
		*tmp++=0xf;
	}
	outb(0x3d4, 12);
	outb(0x3d5, (orgin-base)/2/256);
	outb(0x3d4, 13);
	outb(0x3d5, (orgin-base)/2%256);
	outb(0x3d4, 14);
	outb(0x3d5, (cur-base)/2/256);
	outb(0x3d4, 15);
	outb(0x3d5, (cur-base)/2%256);
	return res;
}
