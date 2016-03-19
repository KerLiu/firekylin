/*
 *    include/firekylin/keyboard.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _KEYBORD_H
#define _KEYBORD_H

#define CTRL_L 	0x81
#define CTRL_R 	0x82
#define SHIFT_L 0x83
#define SHIFT_R 0x84
#define ALT_L   0x85
#define ALT_R   0x86
#define F1  	0x87
#define F2  	0x88
#define F3  	0x89
#define F4  	0x8a
#define F5  	0x8b
#define F6  	0x8c
#define F7  	0x8d
#define F8  	0x8e
#define F9  	0x8f
#define F10 	0x90
#define F11 	0x91
#define F12 	0x92
#define CAPLK 	0x93

#ifdef  _KEYTABLE
unsigned char keytable[]=
{
	0,0,                    //0
	0x1b,0x1b,//0x01	ecs
	'1','!',  //0x02
	'2','@',  //0x03
	'3','#',  //0x04
	'4','$',  //0x05
	'5','%',  //0x06
	'6','^',  //0x07
	'7','&',  //0x08
	'8','*',  //0x09
	'9','(',  //0x0a
	'0',')',  //0x0b
	'-','_',  //0x0c
	'=','+',  //0x0d
	'\b','\b',//0x0e
	'\t','\t',//0x0f
	'q','Q',  //0x10
	'w','W',  //0x11
	'e','E',  //0x12
	'r','R',  //0x13
	't','T',  //0x14
	'y','Y',  //0x15
	'u','U',  //0x16
	'i','I',  //0x17
	'o','O',  //0x18
	'p','P',  //0x19
	'[','{',  //0x1a
	']','}',  //0x1b
	'\n','\n',//0x1c  enter
	CTRL_L,CTRL_L,//0x1d  ctrl_l
	'a','A',  //0x1e
	's','S',  //0x1f
	'd','D',  //0x20
	'f','F',  //0x21
	'g','G',  //0x22
	'h','H',  //0x23
	'j','J',  //0x24
	'k','K',  //0x25
	'l','L',  //0x26
	';',':',  //0x27
	'\'','\"',//0x28
	'`','~',  //0x29
	SHIFT_L,SHIFT_L,//0x2a
	'\\','|', //0x2b
	'z','Z',  //0x2c
	'x','X',  //0x2d
	'c','C',  //0x2e
	'v','V',  //0x2f
	'b','B',  //0x30
	'n','N',  //0x31
	'm','M',  //0x32
	',','<',  //0x33
	'.','>',  //0x34
	'/','?',  //0x35
	SHIFT_R,SHIFT_R,//0x36
	'*','*',  //0x37
	ALT_L,ALT_L,//0x38
	' ',' ',  //0x39
	CAPLK,CAPLK,//0x3a
	F1,F1,	  //0x3b
	F2,F2,	  //0x3c
	F3,F3,	  //0x3d
	F4,F4,	  //0x3e
	F5,F5,	  //0x3f
	F6,F6,	  //0x40
	F7,F7,	  //0x41
	F8,F8,	  //0x42
	F9,F9,	  //0x43
	F10,F10,  //0x44
	0,0,	  //0x45
	0,0,	  //0x46
	'7','7',  //0x47
	'8','8',  //0x48
	'9','9',  //0x49
	'-','-',  //0x4a
	'4','4',  //0x4b
	'5','5',  //0x4c
	'6','6',  //0x4d
	'0','0',  //0x4e
	'1','1',  //0x4f
	'2','2',  //0x50
	'3','3',  //0x51
	'0','0',  //0x52
	'.','.',  //0x53
	0,0,	  //0x54
	0,0,	  //0x55
	0,0,	  //0x56
	F11,F11,  //0x57
	F12,F12,  //0x58
};
#else 
extern unsigned char keytable[];
#endif //KEYTABLE

#endif
