; Copyright (c) 2021. kms1212(Minsu Kwon)
; This file is part of OpenFSL.
; 
; OpenFSL and its source code is published over BSD 3-Clause License.
; See the BSD-3-Clause for more details.
; <https://raw.githubusercontent.com/kms1212/OpenFSL/main/LICENSE>

[BITS 16]
[ORG 0x7C00]

; Code
[SECTION .text]

global _start
_start:
    cli
    xor ax, ax
    mov ds, ax
    cld
    
    mov si, stub_message
    call print_str
    mov ah, 0x00
    int 0x16
    
    cmp al, 0x1B
    je int19
    
int18:
    int 0x18
    jmp halt
    
int19:
    int 0x19
    jmp halt
    
halt:
    hlt
    jmp halt
    
print_str:
    pusha
    .printloop:
        lodsb
        
        or al, al
        jz .return
        
        mov ah, 0x0E
        mov bh, 0x00
        mov bl, 0x07
        
        int 0x10
        jmp .printloop
        
    .return:
        popa
        ret

; Data
stub_message:
db 0x0D, 0x0A, " 1. Press Esc to reboot."
db 0x0D, 0x0A, " 2. Press any other key to boot from other disk.", 0x0D, 0x0A, 0x00
