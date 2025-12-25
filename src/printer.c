#include "printer.h"
#include "types.h"
#include "vga.h"

#define SCREEN_HEIGHT 25
#define SCREEN_WIDE 80

static size_t screen_x_pos = 0;
static size_t screen_y_pos = 0;

void init_printer() { vga_clear_screen(); }

void scroll_if_needed() {
  if (screen_y_pos >= SCREEN_HEIGHT) {
    vga_scroll_down();
    screen_y_pos -= 1;
  }
}

void next_line_if_needed() {
  if (screen_x_pos >= SCREEN_WIDE) {
    screen_x_pos = 0;
    screen_y_pos += 1;
  }
  scroll_if_needed();
}

void vprintf(const char *fmt, va_list args) {

  while (*fmt) {
    if (*fmt == '%') {
      fmt++;
      switch (*fmt) {
      case 'd': {
        int value = va_arg(args, int);
        char num_buf[32];
        itoa(value, num_buf, 10);
        print_string(num_buf);
        break;
      }
      case 'x': {
        int value = va_arg(args, int);
        char hex_buf[32];
        uitoa(value, hex_buf, 16);
        print_string(hex_buf);
        break;
      }
      case 'c': {
        char value = (char)va_arg(args, int);
        print_char(value);
        break;
      }
      case 's': {
        const char *value = va_arg(args, const char *);
        print_string(value);
        break;
      }
      case 'z': {
        size_t value = va_arg(args, size_t);
        char z_buf[32];
        uitoa(value, z_buf, 10);
        print_string(z_buf);
        break;
      }
      case 'p': {
        const void *value = va_arg(args, const void *);
        char p_buf[34];
        ptoa(value, p_buf);
        print_string(p_buf);
        break;
      }
      default:
        print_char('%');
        print_char(*fmt);
        break;
      }
    } else {
      print_char(*fmt);
    }
    fmt++;
  }
  // print_char('\0');
}

void print_char(char ch) {
  next_line_if_needed();
  if (ch == '\n') {
    screen_x_pos = 0;
    screen_y_pos += 1;
  } else {
    vga_print_char(ch, screen_x_pos, screen_y_pos);
    screen_x_pos++;
  }
  next_line_if_needed();
}

void print_string(const char *str) {
  size_t len = strlen(str);
  for (size_t i = 0; i < len; i++) {
    print_char(str[i]);
  }
}

void printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}
