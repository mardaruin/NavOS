#ifndef PRINTER_H
#define PRINTER_H

#include <stdarg.h>

extern void init_printer();
extern void vprintf(const char* fmt, va_list args);
extern void printf(const char* fmt, ...);

#endif