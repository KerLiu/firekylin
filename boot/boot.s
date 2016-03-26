;/*
; *    boot/boot.s
; *
; *    Copyright (C) 2016 ximo<ximoos@foxmail.com>
; *
; * Load kernel to 0x10000, change to protect mode,
; * then jmp kernel enter ponit 0x8:0x10000.
; */

	bits 16
start:
	mov ax,cs
	mov ds,ax
	mov ss,ax
	mov sp,0xfff0
	mov cx,10
	mov di,info+0x7c00
	mov ah,0x0e
	mov bx,0x10
.next:
	mov al,[di]
	int 0x10
	inc di
	loop .next
load_kernel:
	mov dx,0     	; DH-_head,DL--driver
	mov cx,2     	; bit 0-5 begin socter number,ch:cyl number.
	mov ax,0x1000
	mov es,ax    	; buf es:bx
.loop1:
	xor bx,bx
	mov ax,0x224 	; ah:cmd 2--read,AL:read number of socters
	int 0x13
	jc  .die
	inc ch
	cmp ch,[size+0x7c00]
	je  .ok_load
	mov ax,es
	add ax,0x20*36
	mov es,ax	
		     	; echo .
	mov  ah,0x0e
	mov  bx,0x10
	mov  al,'.'
	int  0x10
	jmp  .loop1

.die:	jmp $
.ok_load:	     	; kill floppy motor
	mov dx,0x3f2
	mov al,0
	out dx,al
		     	; echo "\n\r"
	mov ah,0x0e
	mov bx,0x10
	mov al,0x0a  	; "\n"
	int 0x10
	mov al,0x0d  	; "\r"
	int 0x10
		     	; get memory size
	mov ah,0x88
	int 0x15
	mov word [0x2000],ax
.open_A20:
	cli
	in  al,  0x92
	or  al,  2
	out 0x92, al
	lgdt [gdt_48+0x7c00]
	mov eax, cr0
	or  eax, 1
	mov cr0, eax
	jmp dword 8:0x10000

info:
	db "Loading..."
	align 8
gdt:
	dd 0,0
	dd 0x0000ffff,0x00cf9a00
	dd 0x0000ffff,0x00cf9200
	dd 0x0000ffff,0x00cbfa00
	dd 0x0000ffff,0x00cbf200
gdt_48:
	dw 0x800-1
	dd gdt+0x7c00

	times 508-($-$$) db 0
size:
	dw  3
	dw  0xaa55
