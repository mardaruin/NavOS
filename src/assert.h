#ifndef ASSERT_H
#define ASSERT_H

#include "panic.h"

#ifndef DEBUG_MODE
  #define DEBUG_MODE 1
#endif


#if DEBUG_MODE
  #define assert(cond)                                                            \
    do {                                                                          \
      if (!(cond)) {                                                              \
        kernel_panic("Assertion failed: %s at %s:%d", #cond, __FILE__, __LINE__); \
      }                                                                           \
    } while (0)
#else
  #define assert(cond) ((void) 0)
#endif

#endif
