#ifndef PANIC_H
#define PANIC_H
                    
#include <stdarg.h>

extern void cli();                            
extern void vkernel_panic(const char* fmt, va_list args);
extern void kernel_panic(const char* fmt, ...);

#endif