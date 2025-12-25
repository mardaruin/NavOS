#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

extern void *memmove(void *dst, const void *src, size_t n);
extern void *memzero(void *dst, size_t n);
void reverse(char *begin, size_t length);

#endif