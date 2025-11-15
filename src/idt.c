#include "idt.h"
#include "types.h"

struct idt_entry idt[NUM_INTERRUPTS];

void idt_set_gate(uint8_t index, uint32_t handler, uint16_t sel, uint8_t flags) {
	idt[index].base_lo = (uint16_t)(handler & 0xffff);
	idt[index].sel = sel;
	idt[index].always0 = 0;
	idt[index].flags = flags;
	idt[index].base_hi = (uint16_t)((handler >> 16) & 0xffff);
}

void idt_load() {
	struct idt_pointer pidt = {
		.limit = sizeof(struct idt_entry) * NUM_INTERRUPTS + 1,
		.base = (uint32_t)idt
	};

	asm volatile (
		"lidt %0;"
		:: "m"(pidt)
	);
}