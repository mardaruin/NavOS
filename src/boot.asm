[BITS 16]
[ORG 0x7C00]

start:
	cli			;interrupts off
	xor ax, ax
	mov ss, ax
	mov ds, ax
	mov sp, 0x7C00

	mov ax, 0x7E0
	mov es, ax
	sti 			;interrupts on

				;print "Hello, World!"
	mov si, message
	call print_string

	jmp hang

hang:
	jmp hang

print_string:
	pusha
.loop:
	lodsb
	or al, al
	jz .done
	mov ah, 0x0E
	mov bh, 0
	int 0x10
	jmp .loop
.done:
	popa
	ret

message db 'Hello, World!', 0

	

times 510-($-$$) db 0
dw 0xAA55
