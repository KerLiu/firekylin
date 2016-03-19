/*
 *    include/signal.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _SIGNAL_H
#define _SIGNAL_H

typedef unsigned long sigset_t;

#define	NR_SIG		32

#define SIGHUP		1
#define SIGINT		2
#define SIGABRT		3
#define SIGPIPE		4
#define SIGALRM		5
#define SIGCHLD		6
#define SIGCONT		7
#define SIGSTOP		8
#define SIGTSTP		9
#define SIGTTIN		10
#define	SIGTTOU		11
#define SIGKILL		12
/*
#define SIGCONT		13
#define SIGSTOP		14
#define SIGTSTP		15
#define SIGTTIN		16
#define SIGTTOU		17
 */
#define SIG_DFL		((void(*)(int))0)
#define SIG_IGN		((void(*)(int))1)

struct sigaction {
	void (*sa_handle)(int);
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_restoter)(void);
};

/* bits of sa_flags */
#define SA_ONSTACK	0x0001	/* deliver signal on alternate stack       */
#define SA_RESETHAND	0x0002	/* reset signal handler when signal caught */
#define SA_NODEFER	0x0004	/* don't block signal while catching it    */
#define SA_RESTART	0x0008	/* automatic system call restart           */
#define SA_SIGINFO	0x0010	/* extended signal handling                */
#define SA_NOCLDWAIT	0x0020	/* don't create zombies                    */
#define SA_NOCLDSTOP	0x0040	/* don't receive SIGCHLD when child stops  */

/* values used by sigprocmask(). */
#define SIG_BLOCK          0	/* for blocking signals        */
#define SIG_UNBLOCK        1	/* for unblocking signals      */
#define SIG_SETMASK        2	/* for setting the signal mask */
#define SIG_INQUIRE        4	/* for internal use only       */

#endif
