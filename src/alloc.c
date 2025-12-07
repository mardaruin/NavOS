#include "alloc.h"

#include "memory.h"
#include "panic.h"
#include "printer.h"


#define ARENA_START 0x7C00
#define ARENA_SIZE 0x77300

static uint8_t* arena = (uint8_t*) ARENA_START;

void* malloc_undead(size_t size, size_t alignment) {
  size_t alligned_size = size;
  if (size % alignment != 0) {
    alligned_size = ((size / alignment) + 1) * alignment;
  }
  uintptr cur_pos          = (uintptr) arena;
  uintptr next_aligned_pos = (cur_pos + alignment - 1) & ~(alignment - 1);
  size_t padding           = next_aligned_pos - cur_pos;
  size_t total_space       = alligned_size + padding;
  scroll_if_needed();
  if ((uintptr) arena + total_space > (uintptr) (ARENA_SIZE + ARENA_START)) {
    kernel_panic("Overflow error.");
  }
  void* ptr = (void*) next_aligned_pos;
  arena += total_space;
  return ptr;
}

void* calloc_undead(size_t size, size_t alignment) {
  void* ptr = malloc_undead(size, alignment);
  if (ptr) {
    memzero(ptr, size);
  }
  return ptr;
}
