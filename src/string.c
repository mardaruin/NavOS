#include "string.h"

size_t strlen(const char* str) {
  size_t len = 0;
  while (*(str++)) {
    len++;
  }
  return len;
}

char* itoa(int value, char* str, int base) {
  static char digits[] = "0123456789ABCDEF";
  char* wstr           = str;
  int sign             = (value < 0 && base == 10);
  if (sign)
    value = -value;
  do {
    *wstr++ = digits[value % base];
    value /= base;
  } while (value);
  if (sign)
    *wstr++ = '-';
  *wstr = '\0';
  reverse(str, wstr - str - 1);
  return str;
}

char* uitoa(unsigned long num, char* str, int base) {
  char* p = str;
  if (num == 0) {
    *p++ = '0';
    *p   = '\0';
    return str;
  }
  char reversed_buffer[64];
  char* r = reversed_buffer;
  while (num > 0) {
    *r++ = "0123456789abcdef"[num % base];
    num /= base;
  }
  *r = '\0';
  while (*--r) {
    *p++ = *r;
  }
  *p = '\0';
  return str;
}

char* ptoa(const void* ptr, char* str) {
  uintptr addr = (uintptr) ptr;
  char* p      = str;
  *p++         = '0';
  *p++         = 'x';
  uitoa(addr, p, 16);
  return str;
}

void reverse(char* begin, size_t length) {
  char temp;
  char* end = begin + length - 1;
  while (begin < end) {
    temp     = *begin;
    *begin++ = *end;
    *end--   = temp;
  }
}
