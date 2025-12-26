#ifndef PRINTER_H
#define PRINTER_H

// #include "vargs.h"
#include "string.h"
#include <stdarg.h>

extern char *itoa(int value, char *str, int base);
extern void init_printer();
extern void vprintf(const char *fmt, va_list args);
extern void printf(const char *fmt, ...);
extern void scroll_if_needed();
extern void next_line_if_needed();
extern void print_char(char ch);
extern void print_string(const char *str);

#endif