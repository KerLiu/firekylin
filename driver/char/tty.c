/*
 *    driver/tty.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/driver.h>
#include <firekylin/tty.h>

extern void con_init(void);
extern void keyboard_init(void);

struct tty_struct console;

int tty_read(dev_t dev,char * buf,off_t off,size_t size)
{
	long left=size;
	char ch;
	dev=MINOR(dev);
	if(dev!=1)
		panic("Not available tty");

	irq_lock();
	while(left){
		if(!isempty(console.raw)){
			GETCH(console.raw,ch);

			if(ch!='\n')
				*buf++=ch;
			else{
				irq_unlock();
				return size-left;
			}
		}else{
			sleep_on(&(console.raw.wait));
			continue ;
		}
		left--;
	}
	irq_unlock();
	return size-left;
}

int tty_write(dev_t dev,char * buf,off_t off,size_t size)
{
	long left=size;
	char ch;
	while(left){
		if(!isfull(console.out)){
			ch=*buf++;
			PUTCH(console.out,ch);
			left--;
			continue;
		}
		console.write(&console);
	}
	console.write(&console);
	return size-left;
}

int tty_ioctl(dev_t dev, int cmd, long arg)
{
	return 0;
}

struct char_dev tty={ "TTY",NULL ,NULL, tty_read ,tty_write, tty_ioctl };

extern int con_write2(struct tty_struct *tty);

void tty_init(void)
{
	con_init();
	keyboard_init();
	console.raw.head=console.raw.tail=console.raw.count=0;
	console.cooked.head=console.cooked.tail=console.cooked.count=0;
	console.out.head=console.out.tail=console.out.count=0;
	console.write=con_write2;
	char_table[DEV_CHAR_TTYX]=&tty;
}
