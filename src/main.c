#include "vga.h"
#include "memory.h"
#include "panic.h"
#include "alloc.h"
#include "printer.h"
#include "assert.h"
#include "types.h"

void kernel_entry() {
    init_printer();
    while (1) {
        void* ptr = malloc_undead(100, 4);
        printf("Allocated at %p\n", ptr);
    }
}