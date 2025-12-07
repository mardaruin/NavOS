#ifndef PANIC_H
#define PANIC_H

#ifndef DEBUG_MODE
  #define DEBUG_MODE 0
#endif

//#include "vargs.h"
#include <stdarg.h>

extern void cli();
extern void vkernel_panic(const char* fmt, va_list args);
//extern void kernel_panic(const char* fmt, ...);

#ifdef DEBUG_MODE
extern void kernel_panic(const char* fmt, ...);
#else
  #define kernel_panic(fmt, ...)
#endif

#endif
