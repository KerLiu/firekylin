/*
 *    kernel/exec.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/mm.h>
#include <firekylin/fs.h>
#include <errno.h>
#include <elf.h>

/* define in file fs/read.c */
extern long exec_load_file(struct inode *inode, struct buffer*bh);

static int count(char **arg)
{
	int count=0;
	while(*arg){
		count++;
		arg++;
	}
	return count;
}

static long copy_string(char **argv,char **envp)
{
	static char *__arg[]={NULL};
	static long __base=0x400FF000;
	
	int argv_count=0;
	int envp_count=0;
	long arg_page;
	char *s,*tmp;
	long *tmpl;
	
	if(!argv)
		argv=__arg;
	if(!envp)
		envp=__arg;
	
	argv_count=count(argv);
	envp_count=count(envp);
	
	arg_page=__va(get_page());
	
	tmpl=(long*)arg_page;
	*tmpl++=argv_count;			/* argc */
	*tmpl++=__base+4*3;     		/* argv */
	*tmpl++=__base+4*(3+argv_count+1);	/* envp */
	
	s=(char*)arg_page+4*( 3+argv_count+1+envp_count+1);
	
	while(*argv){
		*tmpl++=__base+(long)s-arg_page;
		tmp=*argv;
		while(*tmp){
			*s++=*tmp++;
		}
		*s++=0;
		argv++;	
	}
	*tmpl++=0;
	
	while(*envp){
		*tmpl++=__base+(long)s-arg_page;
		tmp=*envp;
		while(*tmp){
			*s++=*tmp++;
		}
		*s++=0;
		envp++;
	}
	*tmpl++=0;
	
	return __pa(arg_page);
}

int sys_exec(char *filename, char **argv, char **envp)
{
	struct inode *inode;
	struct buffer *buf;
	struct task * current;
	long entry;
	long arg_page;

	current = CURRENT_TASK();

	if (!(inode = namei(filename, NULL)))
		return -ENOENT;

	if (!(inode->i_zone[0]))
		return -ENOEXEC;

	if (!(buf = bread(inode->i_dev, inode->i_zone[0])))
		return -EIO;

	if (!strncmp(buf->b_data, "\0x7FELF", 4))
		return -ENOEXEC;

	arg_page=copy_string(argv,envp);

	free_mm();

	entry=exec_load_file(inode,buf);

	map_page(0x40100000 - 4096, arg_page, current->pdtr);

	iput(inode);

	current->stack = 0x40FF0000;

	asm("movl %%eax,%%cr3"::"a"(current->pdtr));
	asm( " pushl $0x23;"
	     " pushl $0x400FF000;"
	     " pushl $0x13200;"
	     " pushl $0x1b;"
	     " pushl %%eax;"
	     " iret"
	     ::"a"(entry));

	return 0;
}
