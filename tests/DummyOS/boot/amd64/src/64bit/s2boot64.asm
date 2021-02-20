[BITS 32]
[SECTION .text]

extern main_64
global _start

_start:
	mov eax, cr4
	or eax, 0x20
	mov cr4, eax
	
	mov eax, 0x100000
	mov cr3, eax
	
	mov ecx, 0xC0000080
	rdmsr
	or eax, 0x0100
	wrmsr
	
	mov eax, cr0
	or eax, 0xE0000000
	xor eax, 0x60000000
	mov cr0, eax
	
[BITS 64]
	push 0x08
	push x64
	retf
x64:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ss, ax
	mov rsp, 0x6FFFF8
	mov rbp, 0x6FFFF8
	
	call main_64
	jmp $