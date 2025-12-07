file .tmp/os.elf
target remote localhost:1234
set disassembly-flavor intel
b *0x7c60
br main
c