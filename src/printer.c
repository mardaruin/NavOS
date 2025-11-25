#include "types.h"
#include "vga.h"
#include "printer.h"

#define SCREEN_HEIGHT 25
#define SCREEN_WIDE 80

static size_t screen_x_pos = 0;
static size_t screen_y_pos = 0;


void init_printer() {
	vga_clear_screen();	
}

void scroll_if_needed() {
    if (screen_y_pos >= 25) {          
        vga_scroll_down();
        screen_y_pos -= 1;
    }
}


void vprintf(const char* fmt, va_list args) {
	char buffer[1024];
	char* buf_ptr = buffer;
	size_t buf_len = sizeof(buffer);
        char* buf_end = buffer + sizeof(buffer) - 1;


	while(*fmt && buf_ptr < buf_end) {
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
					itoa(value, buf_ptr, 16);
					buf_ptr += strlen(buf_ptr);
					break;
				}
				case 'c': {
					char value = (char)va_arg(args, int);
					*buf_ptr++ = value;
					break;
				}
				case 's': {
					const char* value = va_arg(args, const char*);
					while (*value) {
						*buf_ptr++ = *value++;
					}
					break;
				}
				case 'z': {
                    			size_t value = va_arg(args, size_t);
                    			uitoa(value, buf_ptr, 10);
                    			buf_ptr += strlen(buf_ptr);
                    			break;
                		}
				case 'p': {
					const void* value = va_arg(args, const void*);
					ptoa(value, buf_ptr);
					buf_ptr += strlen(buf_ptr);
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
	*buf_ptr = '\0';
                            
	for (const char* p = buffer; *p && p < buffer + sizeof(buffer); ++p) {
        	if (*p == '\n') {
            		screen_y_pos ++;
            		screen_x_pos = 0;
        	} else if (*p == '\r') {
            		screen_x_pos = 0;
        	} else {
            		vga_print_char(*p, screen_x_pos, screen_y_pos);
            		screen_x_pos++;
            		if (screen_x_pos >= SCREEN_WIDE) {
                		screen_x_pos = 0;
                		screen_y_pos++;
                		scroll_if_needed();
			
            		}
        	}
		scroll_if_needed();
    	}

}

void printf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

}
