#include "memory.h"
#include "types.h"

void *memmove(void *dst, const void *src, size_t n) {
  char *d = (char *)dst;
  const char *s = (const char *)src;
  if (d < s) {
    for (size_t i = 0; i < n; ++i) {
      d[i] = s[i];
    }
  } else {
    for (size_t i = n; i > 0; --i) {
      d[i - 1] = s[i - 1];
    }
  }
  return dst;
}

void *memset(void *dst, int value, size_t n) {
  char *d = (char *)dst;
  for (size_t i = 0; i < n; ++i) {
    d[i] = value;
  }
  return dst;
}

void *memzero(void *dst, size_t n) {
  memset(dst, 0, n);
  return dst;
}

void reverse(char *begin, size_t length) {
  char temp;
  char *end = begin + length - 1;
  while (begin < end) {
    temp = *begin;
    *begin++ = *end;
    *end-- = temp;
  }
}
