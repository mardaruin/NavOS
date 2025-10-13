#include <stdarg.h>
#include "types.h"
#include "vga.h"
#include "printer.h"


void init_printer() {
	vga_clear_screen();	
}

void vprintf(const char* fmt, va_list args) {
	char buffer[1024];
	char* buf_ptr = buffer;
	size_t buf_len = sizeof(buffer);

	while(*fmt) {
		if (*fmt == '%') {
			fmt++;
			switch(*fmt) {
				case 'd': {
					int value = va_arg(args, int);
					itoa(value, buf_ptr, 10);
					buf_ptr += strlen(buf_ptr);
					break;
				}
				case 'x': {
					int value = va_arg(args, int);
					itoa(value, buf_ptr, 10);
					buf_ptr += strlen(buf_ptr);
					break;
				}
				case 'c': {
					char value = (char)va_arg(args, int);
					*buf_ptr++ = value;
					break;
				}
				case 's': {
					const char value = (char)va_arg(args, int);
					while (*value) {
						*buf_ptr++ = *value++;
					}
					break;
				}
				default:                         
					*buf_ptr = *fmt;
					break;
				
			}
		} else {
			*buf_ptr++ = *fmt;
		}
		fmt++;
	}
	*buf_ptr++ = '\0';

	size_t x = 0, y = 0;
	for (const char* p = buffer; *p; ++p) {
        	if (*p == '\n') {
            		y++;
            		x = 0;
        	} else if (*p == '\r') {
            		x = 0;
        	} else {
            		vga_print_char(*p, x, y);
            		x++;
            		if (x >= 80) {
                		x = 0;
                		y++;
                		if (y >= 25) {
                    			vga_scroll_down();
                    			y = 24;
                		}
            		}
        	}
    	}

}

void printf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

}