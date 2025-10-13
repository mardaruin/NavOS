#include "types.h"
#include "panic.h"
#include "printer.h"

void vkernel_panic(const char* fmt, va_list args) {
	cli();
	printf("Kernel panic: ");
	vprintf(fmt, args);
	while(1);
}

void kernel_panic(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vkernel_panic(fmt, args);
	va_end(args);
}