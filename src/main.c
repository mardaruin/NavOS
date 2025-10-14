#include "vga.h"
#include "memory.h"
#include "panic.h"
#include "alloc.h"
#include "printer.h"
#include "assert.h"
#include "types.h"
#include "string.h"

#define START_ADDR (0x7C00 + KERNEL_SIZE)
#define ARENA_END_ADDR (START_ADDR + ARENA_SIZE)

#define SCREEN_HEIGHT 25

static size_t screen_y_pos = 0;


void main() {
	init_printer();

	printf("Memory allocation starts at address: %p\n", (void*)START_ADDR);
	while (1) {
		for(int i = 0; i < 100; i++) {                          
			int size = 123;
			int align = 16;

			void* ptr = malloc_undead((size_t)size, (size_t)align);

			if(ptr) {
				printf("Allocated block of size %d bytes at address %p aligned by %d.\n", size, ptr, align);

				
				screen_y_pos++;
                		scroll_if_needed();

				assert(((uintptr_t)ptr % align == 0));                                     
			} else {
				kernel_panic("Out of memory!");
			}
		}
			 
	}
}                       