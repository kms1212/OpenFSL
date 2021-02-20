[ORG 0x00]
[BITS 16]

[SECTION .text]

VMEMADDR	equ	0xB800

ENTRY:

jmp 0x07E0:START

START:
	mov ax, 0x07E0
	mov ds, ax
	mov es, ax
	mov si, stringStage2
	call PRINT
	
	call ACTA20
	mov si, stringA20
	call PRINT

	call CLEAR
	
	cli
	lgdt [GDTR]

	mov eax, 0x4000003B
	mov cr0, eax

	jmp dword 0x18:(PROTMODE - $$ + 0x7E00)
CLEAR:
	pusha

	mov ah, 0x01
	mov ch, 0x3F
	int 0x10

	mov ah, 0x06
	mov bh, 0x07
	xor cx, cx
	mov dx, 0x184F
	int 0x10
	
	popa
	ret
ACTA20:
	pusha
	mov ax, 0x2401
	int 0x15
	jc .ERR
	jmp .END
.ERR:
	in al, 0x92
	or al, 0x02
	and al, 0xFE
	out 0x92, al
.END:
	popa
	ret
PRINT:
	pusha
.NEXT:
	lodsb
	or al, al
	jz .END

	mov ah, 0x0E
	xor bx, bx
	int 0x10
	jmp .NEXT
.END:
	popa
	ret

[BITS 32]
PROTMODE:
	mov ax, 0x20
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ss, ax
	mov esp, 0xFFFE
	mov ebp, 0xFFFE
	
	mov ebx, (stringProtModeSuccess - $$ + 0x7E00)
	mov ah, 0x00
	mov al, 0x00
	call PRINT32
	add esp, 12

	jmp dword 0x18:0x8000

PRINT32:
	pushad

	push ebx
	
	xor bx, bx
	mov bl, al
	push bx

	xor bx, bx
	mov bl, ah
	push bx
	
	xor eax, eax
	pop ax
	mov esi, 0xA0
	mul esi
	mov edi, eax

	xor eax, eax
	pop ax
	mov esi, 2
	mul esi
	add edi, eax
	
	pop esi
.NEXT:
	mov cl, byte [esi]
	cmp cl, 0
	je .END

	mov byte [edi + 0xB8000], cl
	add esi, 1
	add edi, 2

	jmp .NEXT
.END:
	popad
	ret

stringStage2 : db 'STAGE 2 OK', 0x0D, 0x0A, 0x00
stringA20 : db 'A20 OK', 0x0D, 0x0A, 0x00
stringProtModeSuccess : db 'PROTECTED MODE OK', 0x00

ALIGN 8, DB 0
DW 0x0000
GDTR:
	DW GDTEND - GDT - 1
	DD (GDT - $$ + 0x7E00)
GDT:
	NULLDESC:
		DW 0x0000
		DW 0x0000
		DB 0x00
		DB 0x00
		DB 0x00
		DB 0x00
	CODEDESC64:
		DW 0xFFFF
		DW 0x0000
		DB 0x00
		DB 0x9A
		DB 0xAF
		DB 0x00
	DATADESC64:
		DW 0xFFFF
		DW 0x0000
		DB 0x00
		DB 0x92
		DB 0xAF
		DB 0x00
	CODEDESC:
		DW 0xFFFF
		DW 0x0000
		DB 0x00
		DB 0x9A
		DB 0xCF
		DB 0x00
	DATADESC:
		DW 0xFFFF
		DW 0x0000
		DB 0x00
		DB 0x92
		DB 0xCF
		DB 0x00
GDTEND:

times 510 - ($ - $$) db 0x0
dw 0x9955