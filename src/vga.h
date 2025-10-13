#include <stddef.h>

#ifndef VGA_H
#define VGA_H

extern void vga_clear_screen();
extern void vga_print_char(char c, size_t x, size_t y);
extern void vga_scroll_down();

#endif