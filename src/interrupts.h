#include "alloc.h"
#include "panic.h"
#include "printer.h"
#include "types.h"

#ifndef INTERRUPTS
  #define INTERRUPTS

  #define VECTORS_AMOUNT 256
  #define DESCRIPTOR_SIZE sizeof(idt_descriptor)
  #define IDT_ALIGNMENT 8
  #define IDT_SIZE (VECTORS_AMOUNT * DESCRIPTOR_SIZE)
  #define TRAMPLIN_SIZE 8


  #pragma pack(push, 1)

typedef struct {
  uint16_t offset_0_15      : 16; // Offset to handler entry point [0, 15] bits
  uint16_t segment_selector : 16; // Code segment of handler entry point
  uint8_t reserved_32_36    : 5;
  uint8_t clear_37_39       : 3;
  uint8_t gate_type         : 4;
  uint8_t clear_44          : 1;
  uint8_t DPL               : 2;  // Descriptor privilege level
  uint8_t P                 : 1;  // Present flag (0 - not present, 1 - present)
  uint16_t offset_16_31     : 16; // Offset to handler entry point [16, 31] bits     `
} idt_descriptor;

typedef struct {
  uint16_t limit : 16;
  idt_descriptor* base;
} IDT;

  #pragma pack(pop)

typedef struct {
  uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi, eip;
  uint16_t gs, fs, es, ds;
  uint16_t cs;
  uint16_t int_vector;
  uint32_t error_code;
  uint32_t eflags;
} interrupt_context;

extern void collect_context();
extern void collect_context_without_error_code();

extern void lidt(IDT* idt_address);

void init_idt();

void universal_handler(interrupt_context* context);


#endif
