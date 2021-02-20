global getCPUID, switch64

[SECTION .text]

getCPUID:
	push ebp
	mov ebp, esp
	push eax
	push ebx
	push ecx
	push edx
	push esi
	
	mov eax, dword[ebp + 8]
	cpuid
	
	mov esi, dword[ebp + 12]
	mov dword[esi], eax
	
	mov esi, dword[ebp + 16]
	mov dword[esi], ebx
	
	mov esi, dword[ebp + 20]
	mov dword[esi], ecx
	
	mov esi, dword[ebp + 24]
	mov dword[esi], edx
	
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret
	
switch64:
	jmp 0x18:0x200000
	jmp $