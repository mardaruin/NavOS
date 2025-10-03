
# =============================================================================
# Variables

# Build tools
NASM = nasm -f bin 
GCC = gcc 



#Flags
GCC_FLAGS = gcc -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
LD = 


# =============================================================================
# Tasks

all: clean build test

.tmp/boot.bin: src/boot.asm
	$(NASM) src/boot.asm -o .tmp/boot.bin

#.tmp/main.o: src/main.c
#	$(GCC) $(GCC_FLAGS) -c src/main.c -o .tmp/main.o
                                                               
#.tmp/os.elf: .tmp/boot.o .tmp/main.o link.ld
#	ld -m elf_i386 -s .tmp/main.o .tmp/boot.o -T link.ld -o .tmp/os.elf

#.tmp/os.bin: .tmp/os.elf
#	objcopy -I elf32-i386 -O binary .tmp/os.elf .tmp/os.bin




boot.img: .tmp/boot.bin
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/boot.bin of=boot.img conv=notrunc
	cat war_and_peace.txt | dd of=boot.img bs=1 seek=512 conv=notrunc
	#echo 'hello world again' | dd of=boot.img bs=1 seek=512 conv=notrunc

build: boot.img

clean:
	rm -f *.img
	rm -rf .tmp
	mkdir .tmp
                                                                                       
debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -s -S &
	gdb

test: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -display gtk

.PHONY: all build clean test debug
