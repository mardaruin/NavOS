global read_from_port
read_from_port:
    mov dx, word [esp + 4]
    in al, dx
    ret

global write_to_port
write_to_port:
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret


global cli
cli:
  cli 
  ret

global sti
sti:
  sti
  ret

global lidt
lidt:
  mov eax, dword [esp + 4]
  lidt [eax]
  ret
          
global setup_registers
setup_registers:
  xor eax, eax
  mov ebx, 1
  mov ecx, 2
  mov edx, 3
  mov edi, 4
  mov esi, 5
  mov ebp, 6
  ret
  
global div_zero
div_zero:
  idiv eax
  ret

global pseudo_syscall
pseudo_syscall:
  int 0x21
  ret
     

global collect_context_without_error_code
collect_context_without_error_code:
  push 0
  jmp collect_context


global collect_context
collect_context:

  push ds
  push es
  push fs
  push gs
  pusha

  cld

  mov ax, data_segment
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebx, esp
  ;sub esp, 16
  and esp, 0xFFFFFFF0
  sub esp, 12
  push ebx

  extern universal_handler
  call universal_handler 

  mov esp, ebx
  popa
  pop gs
  pop fs
  pop es
  pop ss
  pop ds
  add esp, 8
  iret

  hlt 
  jmp $

null_segment equ 0
code_segment equ 0x08
data_segment equ 0x10
