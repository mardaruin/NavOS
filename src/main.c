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
	kernel_panic("Out of memory!");
}                       