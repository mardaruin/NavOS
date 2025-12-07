#include "panic.h"

#include "printer.h"
#include "types.h"

void vkernel_panic(const char* fmt, va_list args) {
  cli();
  printf("Kernel panic:\n");
  scroll_if_needed();
  vprintf(fmt, args);
  scroll_if_needed();
  while (1)
    ;
}

void kernel_panic(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vkernel_panic(fmt, args);
  va_end(args);
}
