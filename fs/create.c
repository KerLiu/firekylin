/*
 *    fs/create.c
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#include <firekylin/kernel.h>
#include <firekylin/fs.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <errno.h>

//int sys_create2(char *pathname)
//{
//	struct inode *dir_inode, *inode;
//	char *basename;
//
//	if (!(dir_inode = namei(pathname, &basename)))
//		return -ENOENT;
//
//	if (!*basename) {
//		iput(dir_inode);
//		return -EINVAL;
//	}
//
//	if ((dir_inode->i_op->mknod(dir_inode, basename, &inode))) {
//		iput(dir_inode);
//		return -1;
//	}
//
//	inode->i_mode = S_IFREG | 0777;
//	inode->i_nlink = 1;
//	inode->i_ctime = current_time();
//	inode->i_flag |= I_DIRTY;
//	iput(dir_inode);
//	iput(inode);
//	return 0;
//}
//
//struct dir_entry s_te[2] = {
//	{ 0, ".\0\0\0\0\0\0\0\0\0\0\0\0\0" },
//	{ 0, "..\0\0\0\0\0\0\0\0\0\0\0\0" }
//};
//
//int sys_mkdir(char *pathname)
//{
//	struct inode *dir_inode, *inode;
//	char *basename;
//
//	if (!(dir_inode = namei(pathname, &basename)))
//		return -ENOENT;
//
//	if (!*basename) {
//		iput(dir_inode);
//		return -EINVAL;
//	}
//
//	if ((dir_inode->i_op->mknod(dir_inode, basename, &inode))) {
//		iput(dir_inode);
//		return -1;
//	}
//
//	inode->i_mode = S_IFDIR | 0777;
//	inode->i_nlink = 1;
//	inode->i_ctime = current_time();
//	inode->i_flag |= I_DIRTY;
//	inode->i_size = 32;
//	s_te[0].ino = inode->i_ino;
//	s_te[1].ino = dir_inode->i_ino;
//	iput(dir_inode);
//
//	struct buffer* buf = bread(inode->i_dev, minix1_wbmap(inode, 0));
//	memcpy(buf->b_data, s_te, sizeof(struct dir_entry)*2);
//	buf->b_flag |= B_DIRTY;
//	brelse(buf);
//	iput(inode);
//	return 0;
//}

int sys_create(char *filename,mode_t mode ,long arg)
{
	struct inode *dir_inode, *inode;
	char *basename;

	if (!(dir_inode = namei(filename, &basename)))
		return -ENOENT;

	if (!*basename) {
		iput(dir_inode);
		return -EINVAL;
	}

	if ((dir_inode->i_op->mknod(dir_inode, basename, &inode))) {
		iput(dir_inode);
		return -1;
	}

	inode->i_mode = mode;
	inode->i_nlink = 1;
	inode->i_ctime = current_time();
	inode->i_flag |= I_DIRTY;
	inode->i_size = 0;

	if(S_ISCHR(inode->i_mode)||S_ISBLK(inode->i_mode))
		inode->i_rdev=arg;

	if(S_ISDIR(inode->i_mode)){
		inode->i_op->link(inode,".",inode);
		inode->i_op->link(inode,"..",dir_inode);
	}

	iput(dir_inode);
	iput(inode);
	return 0;
}

int sys_rename(char *old, char *new)
{
	struct inode *old_dir_inode, *new_dir_inode;
	char *old_basename, *new_basename;
	int res;

	old_dir_inode = namei(old, &old_basename);
	if (!old_dir_inode)
		return -ENOENT;
	iunlock(old_dir_inode);
	new_dir_inode = namei(new, &new_basename);
	if (!old_dir_inode) {
		iput(ilock(old_dir_inode));
		return -ENOENT;
	}

	if (old_dir_inode != new_dir_inode) {
		iput(new_dir_inode);
		iput(ilock(old_dir_inode));
		return -1;
	}

	res=new_dir_inode->i_op->rename(new_dir_inode, old_basename,
			new_basename);
	iput(new_dir_inode);
	iput(ilock(old_dir_inode));
	return res;
}

int sys_remove(char *name)
{
	struct inode *inode;
	char *basename;
	int res;

	inode=namei(name,&basename);

	if(!inode)
		return -ENOENT;
	if(!*basename){
		iput(inode);
		return -EINVAL;
	}
	res=inode->i_op->remove(inode,basename);
	iput(inode);

	return res;
}

int sys_link(char *name,char *newname)
{
	struct inode *inode,*new_dir_inode;
	char *basename;
	int res;

	inode=namei(name,NULL);
	if(!inode)
		return -ENOENT;
	if(S_ISDIR(inode->i_mode)){
		iput(inode);
		return -EPERM;
	}
	iunlock(inode);
	new_dir_inode=namei(newname,&basename);
	if(!new_dir_inode){
		iput(ilock(inode));
		return -EACCESS;
	}
	if(!*basename){
		iput(ilock(inode));
		iput(new_dir_inode);
		return -EACCESS;
	}
	if(inode->i_dev!=new_dir_inode->i_dev){
		iput(ilock(inode));
		iput(new_dir_inode);
		return -EACCESS;
	}

	res=new_dir_inode->i_op->link(new_dir_inode,basename,inode);
	iput(ilock(inode));
	iput(new_dir_inode);
	return res;
}
