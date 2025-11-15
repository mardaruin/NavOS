#ifndef IDT_H
#define IDT_H

#include "types.h"

#define NUM_INTERRUPTS 256

struct idt_entry {
	uint16_t base_lo;
	uint16_t sel;
	uint8_t always0;
	uint8_t flags;
	uint16_t base_hi;
};

struct idt_pointer {
	uint16_t limit;
	uint32_t base;
};

void idt_set_gate(uint8_t index, uint32_t handler, uint16_t sel, uint8_t flags);

void idt_load();

#endif