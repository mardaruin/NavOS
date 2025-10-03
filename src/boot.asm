[BITS 16]
[ORG 0x7C00]

start:                                        
	cli   
	xor ax, ax   
	mov ss, ax              ; SS=0x0000 (stack segment)
	mov ds, ax              ; DS=0x0000 (data segment)
	mov sp, 0x7C00          ; SP=0x07C0 (stack pointer)

	mov di, 0x7E0           ; DI=0x7E0 (целевой сегмент)
	mov es, di              ; ES=DI (адрес, куда будем загружать данные)
	xor bx, bx              ; BX=0x0000 (смещение внутри сегмента)

	mov di, NUMBER_OF_SECTORS

	mov al, 1               ;1 sector
	xor ch, ch              ;0 cylinder
	mov cl, 2               ;2 sector (after loader)
	xor dh, dh              ;head 0

read_loop:
	mov ah, 0x02            ; bios функция для чтения с диска
	int 0x13

	jc disk_read_failed

	add bx, 512
	inc cl 
	dec di
	jne read_loop

	mov si, success_msg
	call print_string

	jmp halt_and_wait

disk_read_failed:
	mov si, error_msg
	call print_string
	jmp halt_and_wait

halt_and_wait:
	hlt
	jmp halt_and_wait

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

NUMBER_OF_SECTORS equ 2

success_msg db 'Data successfully loaded', 0
error_msg db 'Disk read error.', 0

times 510-($-$$) db 0
dw 0xAA55
