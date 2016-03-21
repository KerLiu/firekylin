/*
 *    include/termios.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _TERMIOS_H
#define _TERMIOS_H

#define NCCS 17
struct termios {
	unsigned long c_iflag;
	unsigned long c_oflag;
	unsigned long c_cflag;
	unsigned long c_lflag;
	unsigned char c_line;
	unsigned char c_cc[NCCS];
};

/* Values for termios c_iflag bit map.  */
#define BRKINT		0x0001	/* signal interrupt on break */
#define ICRNL		0x0002	/* map CR to NL on input */
#define IGNBRK		0x0004	/* ignore break */
#define IGNCR		0x0008	/* ignore CR */
#define IGNPAR		0x0010	/* ignore characters with parity errors */
#define INLCR		0x0020	/* map NL to CR on input */
#define INPCK		0x0040	/* enable input parity check */
#define ISTRIP		0x0080	/* mask off 8th bit */
#define IXOFF		0x0100	/* enable start/stop input control */
#define IXON		0x0200	/* enable start/stop output control */
#define PARMRK		0x0400	/* mark parity errors in the input queue */

/* Indices into c_cc array.  Default values in parentheses. POSIX Table 7-5. */
#define VEOF                 0	/* cc_c[VEOF] = EOF char (^D) */
#define VEOL                 1	/* cc_c[VEOL] = EOL char (undef) */
#define VERASE               2	/* cc_c[VERASE] = ERASE char (^H) */
#define VINTR                3	/* cc_c[VINTR] = INTR char (DEL) */
#define VKILL                4	/* cc_c[VKILL] = KILL char (^U) */
#define VMIN                 5	/* cc_c[VMIN] = MIN value for timer */
#define VQUIT                6	/* cc_c[VQUIT] = QUIT char (^\) */
#define VTIME                7	/* cc_c[VTIME] = TIME value for timer */
#define VSUSP                8	/* cc_c[VSUSP] = SUSP (^Z, ignored) */
#define VSTART               9	/* cc_c[VSTART] = START char (^S) */
#define VSTOP               10	/* cc_c[VSTOP] = STOP char (^Q) */

struct winsize
{
	unsigned short	ws_row;		/* rows, in characters */
	unsigned short	ws_col;		/* columns, in characters */
	unsigned short	ws_xpixel;	/* horizontal size, pixels */
	unsigned short	ws_ypixel;	/* vertical size, pixels */
};

#endif
