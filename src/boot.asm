[BITS 16]
[ORG 0x7C00]

start:                                        
	cli   
	xor ax, ax   
	mov ss, ax              ; SS=0x07C0 (stack segment)
	mov ds, ax              ; DS=0x07C0 (data segment)
	mov sp, 0x7C00              ; SP=0x0000 (stack pointer)

	mov di, 0x7E0           ; DI=0x7E0 (целевой сегмент)
	mov es, di              ; ES=DI (адрес, куда будем загружать данные)
	xor bx, bx              ; BX=0x0000 (смещение внутри сегмента)

	mov ah, 0x02            ;BIOS: memoory reading	mov ch, 0 
	mov al, 1               ;1 sector
	mov ch, 0               ;0 cylinder
	mov cl, 2               ;2 sector (after loader)
	mov dh, 0               ;head 0
	mov dl, 0               ;fda

	;reading sector
	int 0x13

	jc disk_read_failed

	mov si, 0x7E00          ; addr loaded data
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

error_msg db 'Disk read error.', 0

	

times 510-($-$$) db 0
dw 0xAA55
