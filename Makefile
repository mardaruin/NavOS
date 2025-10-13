
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
                                                                



boot.img: .tmp/boot.bin
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/boot.bin of=boot.img conv=notrunc
	cat random_big_file.txt | dd of=boot.img bs=1 seek=512 conv=notrunc
	#echo 'hello world again' | dd of=boot.img bs=1 seek=512 conv=notrunc

build: boot.img

dump:                                                                 
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA
# -dump-guest-core dump.bin

clean:
	rm -f *.img
	rm -rf .tmp
	mkdir .tmp
                                                                                       
debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -s -S &
	gdb
	target remote localhost:1234

test: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -display gtk

.PHONY: all build clean test debug
