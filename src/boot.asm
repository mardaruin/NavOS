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
  mov eax, tss
  mov word [tss_desc + 2], ax
  shr eax, 16
  mov byte [tss_desc + 4], al
  mov byte [tss_desc + 7], ah
  mov ax, tss_segment
  ltr ax
  

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


align 8
tss:
  .previous_task_link: dd 0
  .esp0:               dd 0x7c00
  .ss0:                dw data_segment
  .reserved0:          dw 0
  .esp1:               dd 0
  .ss1:                dw 0
  .reserved1:          dw 0
  .esp2:               dd 0
  .ss2:                dw 0
  .reserved2:          dw 0
  .cr3:                dd 0
  .eip:                dd 0
  .eflags:             dd 0
  .eax:                dd 0
  .ecx:                dd 0
  .edx:                dd 0
  .ebx:                dd 0
  .esp:                dd 0
  .ebp:                dd 0
  .esi:                dd 0
  .edi:                dd 0
  .es:                 dw 0
  .reserved3:          dw 0
  .cs:                 dw 0
  .reserved4:          dw 0
  .ss:                 dw 0
  .reserved5:          dw 0
  .ds:                 dw 0
  .reserved6:          dw 0
  .fs:                 dw 0
  .reserved7:          dw 0
  .gs:                 dw 0
  .reserved8:          dw 0
  .ldt_selector:       dw 0
  .reserved9:          dw 0
  .debug_trap:         dw 0
  .io_map_base:        dw 108
  .ssp:                dd 0


gdt_descriptor:
  dw gdt_end - gdt - 1    ; размер таблицы gdt - 1
  dd gdt                  ; лин адрес самой таблицы


align 8       ; выравнивание
gdt:
  dq 0                    ; по стандарту первая запись в gdt всегда должна быть 0
.code_segment_desc:
  dw 0xffff               ; low limit - максимальный размер сегмента
  dw 0                    ; low base address
  db 0                    ; middle base address
  db 0b10011010           ; access bytes - p=1, dpl=00, s=1, type=1010 
  db 0b11001111           ; granularity byte - g=1, db=1, l=0, avl=0, high bits of the limit=1111 
  db 0                    ; high base address
.data_segment_desc:
  dw 0xffff               ; low limit - максимальный размер сегмента
  dw 0                    ; low base address
  db 0                    ; middle base address
  db 0b10010010           ; access bytes - p=1, dpl=00, s=1, type=0010 
  db 0b11001111           ; granularity byte - g=1, db=1, l=0, avl=0, high bits of the limit=1111 
  db 0x0                  ; high base address
.user_code_segment_desc:
; code segment with dpl = 3
  dw 0xffff               ; low limit - максимальный размер сегмента
  dw 0                    ; low base address
  db 0                    ; middle base address
  db 0b11111010           ; access bytes - p=1, dpl=11, s=1, type=1010 
  db 0b11001111           ; granularity byte - g=1, db=1, l=0, avl=0, high bits of the limit=1111 
  db 0                    ; high base address
.user_data_segment_desc:
; data segment with dpl = 3
  dw 0xffff               ; low limit - максимальный размер сегмента
  dw 0                    ; low base address
  db 0                    ; middle base address
  db 0b11110010           ; access bytes - p=1, dpl=11, s=1, type=0010 
  db 0b11001111           ; granularity byte - g=1, db=1, l=0, avl=0, high bits of the limit=1111 
  db 0x0                  ; high base address
tss_desc:           
  dw 0x6b                 ; low limit - 1
  dw 0                    ; low base addr
  db 0                    ; middle base address 
  db 0b10001001           ; access bytes - p=1, dpl=00, s=0, type=1001 
  db 0                    ; granularity byte - g=0, db=0, l=0, avl=0, high bits of the limit=0000
  db 0                    ; high base address
gdt_end:


tss_segment equ 40        ; ind = 101, gdt, pl=00


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
