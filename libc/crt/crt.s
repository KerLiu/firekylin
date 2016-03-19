;/*
; *    libc/crt/crt.s
; *
; *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
; */

extern main
global _start,environ

section .text
_start:
	mov  ax,ss
	mov  ds,ax
	mov  es,ax
	mov  eax,[esp+8]
	mov  dword [environ],eax
	call main
	mov  eax,4
	int  0x30
	
section .data
environ:
	dd 0
