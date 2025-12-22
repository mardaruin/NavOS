[BITS 16]


start:                                        
  cli   
  xor ax, ax   
  mov ss, ax              ; SS=0x0000 (stack segment)
  mov ds, ax              ; DS=0x0000 (data segment)
  mov sp, 0x7C00          ; SP=0x07C0 (stack pointer)

  mov di, 0x7E0           ; DI=0x7E0 (целевой сегмент)
  mov es, di              ; ES=DI (адрес, куда будем загружать данные)
  xor bx, bx              ; BX=0x0000 (смещение внутри сегмента)

  mov si, NUMBER_OF_SECTORS

  mov al, 1               ;1 sector
  xor ch, ch              ;0 cylinder
  mov cl, 2               ;2 sector (after loader)
  xor dh, dh              ;head 0

read_loop:
  mov ah, 0x02            ; bios функция для чтения с диска
  int 0x13

  jc disk_read_failed

  dec si
  jz read_end

  mov di, es
  add di, 0x20
  mov es, di

  inc cl
  cmp cl, MAX_SECTORS
  jle read_loop
  mov cl, 1

  inc dh
  cmp dh, MAX_HEADS
  jle read_loop
  xor dh, dh

  inc ch
  jmp read_loop

                         

disk_read_failed:
  mov si, error_msg
  call print_string
  jmp inf_loop

;halt_and_wait:
;  hlt
;  jmp halt_and_wait

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

read_end:
  mov si, success_msg
  call print_string  
                 

  lgdt [gdt_descriptor]
  cld

  ; включаем защиту и переводим в защищенный режим
  mov eax, cr0
  or eax, 1       ; устанавливаем флаг PE
  mov cr0, eax
  
  jmp code_segment:code_start      ; дальний прыжок


[BITS 32]
code_start:
  mov ax, data_segment
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  extern kernel_entry
  call kernel_entry


global inf_loop
inf_loop:
  jmp inf_loop


gdt_descriptor:
  dw gdt_end - gdt - 1    ; размер таблицы gdt - 1
  dd gdt                  ; лин адрес самой таблицы

align 8       ; выравнивание
gdt:
  dq 0                    ; по стандарту первая запись в gdt всегда должна быть 0
; кодовый сегмент 
  dw 0xffff               ; low limit - максимальный размер сегмента
  dw 0                    ; low base address
  db 0                    ; middle base address
  db 0b10011010           ; access bytes - p=1, dpl=00, s=1, type=1010 
  db 0b11001111           ; granularity byte - g=1, db=1, l=0, avl=0, high bits of the limit=1111 
  db 0                    ; high base address
; сегмент данных
  dw 0xffff
  dw 0
  db 0
  db 0b10010010
  db 0b11001111
  db 0x0
gdt_end:

null_segment equ 0
code_segment equ 0x08
data_segment equ 0x10


NUMBER_OF_SECTORS equ 400
MAX_SECTORS equ 18
MAX_HEADS equ 1
MAX_CYLINDERS equ 79

success_msg db 'Data successfully loaded', 0
error_msg db 'Disk read error.', 0


times 510-($-$$) db 0
dw 0xAA55
