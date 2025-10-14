#ifndef PRINTER_H
#define PRINTER_H

//#include "vargs.h"
#include <stdarg.h>
#include "string.h"
                    
extern char* itoa(int value, char* str, int base);
extern void init_printer();
extern void vprintf(const char* fmt, va_list args);
extern void printf(const char* fmt, ...);
extern void scroll_if_needed();

#endif