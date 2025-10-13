#include <stddef.h>

#ifndef ALLOC_H
#define ALLOC_H

extern void* malloc_undead(size_t size, size_t alignment);
extern void* calloc_undead(size_t size, size_t alignment);

#endif