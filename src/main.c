#include "vga.h"
#include "memory.h"
#include "panic.h"
#include "alloc.h"
#include "printer.h"
#include "assert.h"
#include "types.h"
#include "string.h"
#include <stdbool.h>

#define START_ADDR (0x7C00 + KERNEL_SIZE)
#define ARENA_END_ADDR (START_ADDR + ARENA_SIZE)

#define DELAY_MS(ms) ({ \
    volatile uint32_t delay = ms * 100000; \
    while(delay--) {} \
})

#define SCREEN_HEIGHT 25
#define BLOCK_SIZE 123
#define ALIGNMENT_VALUE 16

static size_t screen_y_pos = 0;


void main() {
	init_printer();

	printf("Memory allocation starts at address: %p\n", (void*)START_ADDR);

	int itarration_count = 0;
	bool out_of_memory = false;

	while (1) {
		//for(int i = 0; i < 100; i++) {                          
			size_t size = BLOCK_SIZE;
			size_t align = ALIGNMENT_VALUE;

			void* ptr = malloc_undead(size, align);

			if(ptr) {
				printf("Allocated block of size %z bytes at address %p aligned by %z.\n", size, ptr, align);

				
				screen_y_pos++;
                		scroll_if_needed();

				assert(((uintptr_t)ptr % align == 0));   
				//DELAY_MS(2);                                     
			} else {
				kernel_panic("Out of memory!");
			}
		//}
			 
	}
}                       