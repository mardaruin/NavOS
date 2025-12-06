[BITS 16]
[ORG 0x7C00]

%define SECTOR_SIZE 512
%define NUMBER_OF_SECTORS ((N / SECTOR_SIZE) + ((N % SECTOR_SIZE) != 0))

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
  mov ah, 0x02
  int 0x13                 ; Вызов BIOS для чтения сектора

  jc disk_read_failed      ; Если ошибка, идём в обработчик ошибок

  dec di                   ; Уменьшаем количество оставшихся секторов
  jz read_end              ; Если секторов больше нет, завершили загрузку успешно

  ; Обновляем значение ES для следующей порции данных
  mov si, es               ; Копируем текущее значение ES в SI
  add si, 0x20             ; Добавляем смещение 0x20
  mov es, si

  inc cl                   ; Переходим к следующему сектору
  cmp cl, MAX_SECTORS      ; Проверяем, достигли ли максимального числа секторов
  jle read_loop            ; Если ещё есть свободные, продолжаем читать

  mov cl, 1                ; Иначе начинаем заново с первого сектора на следующей головке

  inc dh                   ; Меняем головк
  cmp dh, MAX_HEADS        ; Проверяем число головок
  jle read_loop            ; Если есть ещё головки, используем следующую

  xor dh, dh               ; Если обе стороны использованы, возвращаемся к первой стороне 

  inc ch                   ; Идём на следующий цилиндр
  jmp read_loop            ; Продолжаем цикл чтения
                         

disk_read_failed:
  mov si, error_msg
  call print_string
  jmp halt_and_wait

halt_and_wait:
  hlt
  jmp halt_and_wait

; Функция print_string выводит строку на экран в режиме реального времени BIOS.
; Аргументы:
;   DS:SI - адрес строки, заканчивающейся нулевым байтом (\0)
print_string:
  ; Сохраняем состояние всех регистров
  pusha
.loop:
  ; Загружаем следующий символ строки в AL
  lodsb

  ; Проверяем конец строки (ноль в AL)
  or al, al
  jz .done        ; Если достигли конца строки, переходим к done

  ; Настраиваем режим вывода символов через прерывания BIOS
  mov ah, 0x0E
  mov bh, 0

  ; Генерируем прерывание BIOS для вывода символа
  int 0x10

  ; Переход обратно к началу цикла для следующего символа
  jmp .loop
.done:
  ; Восстанавливаем сохранённые регистры
  popa

  ; Возвращаемся из процедуры
  ret

read_end:
  mov si, success_msg
  call print_string
    
  jmp halt_and_wait

  

;NUMBER_OF_SECTORS equ 400
MAX_SECTORS equ 18
MAX_HEADS equ 1
MAX_CYLINDERS equ 79

success_msg db 'Data successfully loaded', 0
error_msg db 'Disk read error.', 0

times 510-($-$$) db 0
dw 0xAA55