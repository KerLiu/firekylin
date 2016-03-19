/*
 *    driver/keyboard.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/trap.h>
#include <firekylin/tty.h>

#define _KEYTABLE
#include <firekylin/keyboard.h>

static int shiftl = 0;
static int shiftr = 0;
static int cap = 0;

extern struct tty_struct console;

void do_keyboard(struct trapframe *tf)
{
	unsigned char tmp, s;
	outb(0x20, 0x20);
	tmp = inb(0x60);
	if (tmp < 0x7f) {
		s = keytable[tmp * 2 + (shiftl | shiftr)];
		switch (s) {
		case CAPLK:
			cap = !cap;
			break;
		case SHIFT_L:
			shiftl = 1;
			break;
		case SHIFT_R:
			shiftr = 1;
			break;
		default:
			if (cap) {
				if (s >= 'a' && s <= 'z')
					s = s - 32;
				else if (s >= 'A' && s <= 'Z')
					s = s + 32;
			}
			printk("%c", s);
			if(!isfull(console.raw)){
				PUTCH(console.raw,s);
				wake_up(&(console.raw.wait));
			}
			break;
		}
	} else {
		if (tmp == 0xe0) {
			tmp = inb(0x60);
		} else {
			tmp &= 0x7f;
			tmp = keytable[tmp * 2];
			switch (tmp) {
			case SHIFT_L:
				shiftl = 0;
				break;
			case SHIFT_R:
				shiftr = 0;
				break;
			}
		}
	}
}

void keyboard_init(void)
{
	set_trap_handle(0x21, &do_keyboard);
	outb(0x21, inb(0x21)&~2);
}
