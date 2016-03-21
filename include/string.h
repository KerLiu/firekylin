/*
 *    include/string.h
 *
 *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
 */

#ifndef _STRING_H
#define _STRING_H

#include <sys/types.h>

extern void*  memset(void *s, int c, size_t n);
extern void*  memcpy(void *dst, void *src, size_t n);
extern void*  memchr(const void *s, int c, size_t n);
extern int    memcmp(const void *s1, const void *s2, size_t n);
extern void*  memmove(void *s1, void *s2, size_t n);

extern char*  strcpy(char *dst, char *src);
extern char*  strcat(char *dst, char *src);
extern size_t strlen(const char *s);
extern char*  strchr(const char *s, int c);
extern int    strcmp(char *s1,char *s2);
extern int    stricmp(const char *s1, const char *s2);

extern char*  strncpy(char *dst, char *src, size_t n);
extern char*  strncat(char *dst, char *src, size_t n);
extern size_t strnlen(const char *s, size_t n);
extern char*  strnchr(const char *s, int c, size_t n);
extern int    strncmp(char *s1, char *s2, size_t n);
extern int    strnicmp(const char *s1, const char *s2, size_t n);

extern char * strerror(int errno);

#endif
