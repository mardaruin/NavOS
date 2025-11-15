#include "types.h"

#ifndef UNIVERSAL_HANDLER
#define UNIVERSAL_HANDLER

#pragma pack(push, 1)

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
  uint16_t fs;
  uint16_t es;
  uint16_t ds;


} context;

void universal_handler(context* ctx);

#endif