;/*
; *    init/head.s
; *
; *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
; */

global  _start
extern  start

section .text
_start:
	mov ax,0x10
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov esp,0x10000
		; set simple page table, map 0~4MB
	xor eax,eax
	xor edi,edi
	mov ecx,4096
	rep stosb
		; PDE
	mov eax,0x100007
	xor edi,edi
	mov [edi],eax
	mov edi,3072
	mov [edi],eax
		; PTE
	mov edi,0x100000
	mov ecx,1024
	mov eax,7
.next:
	mov [edi],eax
	add edi,4
	add eax,0x1000
	loop .next

	xor eax,eax
	mov cr3,eax
	mov eax,cr0
	or  eax,0x80000000
	mov cr0,eax
	mov esp,0xc0010000
	push start
	ret
