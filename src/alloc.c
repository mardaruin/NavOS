#include "types.h"
#include "panic.h"
#include "alloc.h"
#include "memory.h"
#include "printer.h"

#define ARENA_START 0x7C00
#define ARENA_SIZE 0x8000

static uint8_t* arena = (uint8_t*)ARENA_START;

void* malloc_undead(size_t size, size_t alignment) {
	size_t alligned_size = size;    
	if (size % alignment != 0){
		alligned_size = ((size / alignment) + 1) * alignment;
	}
	printf("Allocating block size %z\n", alligned_size);
	scroll_if_needed();
	if ((uintptr_t)arena + alligned_size > (uintptr_t)(ARENA_SIZE + ARENA_START)) {
		kernel_panic("Kolobok hanged himself.");
	}
	void* ptr = arena;
	arena += alligned_size;
	return ptr;
}

void* calloc_undead(size_t size, size_t alignment) {
	void* ptr = malloc_undead(size, alignment);
	if (ptr) {
		memzero(ptr, size);
	}
	return ptr;
}