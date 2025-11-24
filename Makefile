# =============================================================================
# Variables

# Build tools
NASM = nasm -f elf
GCC = gcc
GCC_FLAGS = -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector 
                                             


# =============================================================================
# Tasks

all: clean build test

.tmp/boot.o: src/boot.asm
	$(NASM) src/boot.asm -o .tmp/boot.o -dN=0xA000
                                                        

.tmp/%.o: src/%.c
	$(GCC) $(GCC_FLAGS) -c $< -o $@

.tmp/os.elf: .tmp/boot.o .tmp/main.o link.ld
	ld -m elf_i386 -s .tmp/boot.o .tmp/string.o .tmp/vga.o .tmp/memory.o .tmp/panic.o .tmp/alloc.o .tmp/printer.o .tmp/main.o .tmp/kernel_entry.o .tmp/interrupts.o  -T link.ld -o .tmp/os.elf

.tmp/os.bin: .tmp/os.elf 
	objcopy -I elf32-i386 -O binary .tmp/os.elf .tmp/os.bin
	

boot.img: .tmp/boot.o .tmp/string.o .tmp/main.o .tmp/vga.o .tmp/memory.o .tmp/panic.o .tmp/alloc.o .tmp/printer.o .tmp/kernel_entry.o .tmp/interrupts.o .tmp/os.bin .tmp/os.elf   
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/os.bin of=boot.img conv=notrunc                      

build: boot.img

clean:
	rm -f *.img
	rm -rf .tmp
	mkdir .tmp

test: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA

debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -s -S &
	gdb

.PHONY: all build clean test debug