#ifndef ASSERT_H
#define ASSERT_H

#include "panic.h"

#define assert(cond) \
    if (!(cond)) { \
        kernel_panic("Assertion failed: %s at %s:%d", #cond, __FILE__, __LINE__); \
    }

#endif