#ifndef TYPES_H
#define TYPES_H

typedef _Bool bool;

#define true 1
#define false 0

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

typedef unsigned long uintptr_t;

typedef unsigned long size_t;

#define NULL ((void *)0)

#define DELAY_MS(ms)                                                           \
  ({                                                                           \
    volatile uint32_t delay = ms * 100000;                                     \
    while (delay--) {                                                          \
    }                                                                          \
  })

#define KERNEL_SIZE 0x10000
#define ARENA_SIZE 0x80000

#define TRAP_GATE_TYPE 0x8F
#define INTR_GATE_TYPE 0x8E

#define CODE_SEGMENT 0x08
#define DATA_SEGMENT 0x10

extern void div_zero();
extern void pseudo_syscall();
extern void setup_registers();
extern void inf_loop();
extern void sti();
extern uint32_t get_esp(void);
extern void not_allowed();

#endif
