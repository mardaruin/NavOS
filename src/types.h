#ifndef TYPES_H
#define TYPE_H

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

#define KERNEL_SIZE 0x10000
#define ARENA_SIZE 0x80000

#define TRAP_GATE_TYPE 0x8F
#define INTR_GATE_TYPE 0x8E

#define CODE_SEGMENT 0x08
#define DATA_SEGMENT 0x10

#endif