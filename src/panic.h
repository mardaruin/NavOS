#ifndef PANIC_H
#define PANIC_H

#ifndef DEBUG_MODE
  #define DEBUG_MODE 0
#endif

//#include "vargs.h"
#include <stdarg.h>

extern void cli();                                           

extern void kernel_panic(const char* fmt, ...);
      
#endif
