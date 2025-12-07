#include "types.h"

#ifndef ALLOC_H
  #define ALLOC_H


void* malloc_undead(size_t size, size_t alignment);
void* calloc_undead(size_t size, size_t alignment);

#endif
