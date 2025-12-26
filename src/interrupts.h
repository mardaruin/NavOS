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
#define TRAMPLIN_SIZE 7

#pragma pack(push, 1)

typedef enum {
  INTERRUPT_GATE = 0xE,
  TRAP_GATE = 0xF,

} GATE_TYPE;

typedef struct {
  uint32_t edi;
  uint32_t esi;

  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;

  uint16_t gs;
  uint16_t padding_1;
  uint16_t fs;
  uint16_t padding_2;
  uint16_t es;
  uint16_t padding_3;
  uint16_t ds;
  uint16_t padding_4;

  uint16_t int_vector;
  uint16_t padding_5;
  uint32_t error_code;

  uint32_t eip;
  uint16_t cs;
  uint16_t padding_6;
  uint32_t eflags;
  // uint16_t ss;
} interrupt_context;

typedef struct {
  uint16_t offset_0_15 : 16;      // Offset to handler entry point [0, 15] bits
  uint16_t segment_selector : 16; // Code segment of handler entry point
  uint8_t reserved_32_36 : 5;
  uint8_t clear_37_39 : 3;
  uint8_t gate_type : 4;
  uint8_t clear_44 : 1;
  uint8_t DPL : 2;            // Descriptor privilege level
  uint8_t P : 1;              // Present flag (0 - not present, 1 - present)
  uint16_t offset_16_31 : 16; // Offset to handler entry point [16, 31] bits `
} idt_descriptor;

typedef struct {
  uint16_t limit : 16;
  idt_descriptor *base;
} IDT;

#pragma pack(pop)

extern void collect_context();
extern void collect_context_without_error_code();

extern void lidt(IDT *idt_address);

void init_idt();
static const char *error_type_message(uint8_t v);

void universal_handler(interrupt_context *context);

typedef enum {
  TIMER,
  KEYBOARD,
} PIC_DEVICES;

#define N (136)

#define MASTER_COMMAND_PORT 0x20
#define MASTER_DATA_PORT 0x21
#define SLAVE_COMMAND_PORT 0xA0
#define SLAVE_DATA_PORT 0xA1

#define SLAVE_IRQ_IN_MASTER 2

#define CONTROLLER_REGISTER 0x60

#define ICW1_CASCADE_MODE (0 << 1)
#define ICW2_MASTER 0x20
#define ICW2_SLAVE 0X28
#define ICW3_MASTER (1 << SLAVE_IRQ_IN_MASTER)
#define ICW3_SLAVE SLAVE_IRQ_IN_MASTER
#define ICW4_FULLY_NESTED_MODE 0x1

#define EOI 0x20

#define DEVICE_MASK_NONE 0xFF
#define DEVICE_MASK_TIMER 0xFE
#define DEVICE_MASK_KEYBOARD 0xFD
#define DEVICE_MASK_BOTH (DEVICE_MASK_TIMER & DEVICE_MASK_KEYBOARD)

extern void write_to_port(uint16_t port, uint8_t message);
extern uint8_t read_from_port(uint16_t port);
extern void setup8259(bool aeoi);
void send_eoi();
void set_device(uint8_t device);
void inf_loop_with_inc();

#endif
