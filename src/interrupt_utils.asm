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


global eflags
eflags:
  pushfd
  pop eax
  ret

global get_esp
get_esp:
  mov eax, esp
  ret

global syscall_expr
syscall_expr:
    mov eax, dword [esp + 4]
    int 0x30
    ret

global not_allowed
not_allowed:
  cli
  ; in al, dx
  ; lgdt [eax]
  ; ligt [eax]
  ; ltr [eax]
  ; mov cr0, eax
  ; inf_loop
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
  push dword [esp]


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
  pop ds
  add esp, 8
  iret

  hlt 
  jmp $

global restore_context
restore_context:
  mov esp, dword [esp + 4]
  popa
  pop gs
  pop fs
  pop es
  pop ds
  add esp, 8
  iret

null_segment equ 0
code_segment equ 0x08
data_segment equ 0x10
