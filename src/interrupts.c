#include "interrupts.h"
#include "vga.h"

#define DELAY_MS(ms) ({ \
    volatile uint32_t delay = ms * 100000; \
    while(delay--) {} \
})


static uint8_t with_error_code[10] = {0x8, 0xA, 0xB, 0xC, 0xD, 0xE, 0x11, 0x15, 0x1d, 0x1e};

static bool has_error_code(uint8_t v) {
  for (uint8_t i = 0; i < 10; i++) {
    if (with_error_code[i] == v) {
      return true;
    }
  }
  return false;
}

void init_idt() {
  uint8_t* tramplins = (uint8_t *) malloc_undead(TRAMPLIN_SIZE * VECTORS_AMOUNT, 1);

  for (uint16_t v = 0; v < VECTORS_AMOUNT; v++) {
    printf("Dealing with %x vector tramplin.\n", v);
    scroll_if_needed();
    DELAY_MS(2);                                                               
    bool v_has_error_code = has_error_code(v);

    uint8_t* tramplin = (uint8_t *)(tramplins + v * TRAMPLIN_SIZE);
    uint32_t offset = 0;

    if (!v_has_error_code) {
      tramplins[offset++] = 0x50; // push eax (60 slide)
    }
    else {
      offset++;
    }
    tramplins[offset++] = 0x6a;   // push imm8
    tramplins[offset++] = v;
    tramplins[offset++] = 0xe9;   // jmp
    uint32_t func_offset = (uint32_t)collect_context - (uint32_t)(tramplin + offset + 4);
    *(uint32_t*)(tramplin + offset) = func_offset;  // collect_context relative address
  }

  DELAY_MS(10);
  scroll_if_needed();


  idt_descriptor* idt = malloc_undead(IDT_SIZE, IDT_ALIGNMENT);
  for (uint16_t v = 0; v < VECTORS_AMOUNT; v++) {
    printf("Filling idt %x vector.\n", v);
    scroll_if_needed();
    idt[v].offset_0_15 = (uint32_t)(tramplins + TRAMPLIN_SIZE * v) & 0xffff;
    idt[v].segment_selector = 0x08;
    idt[v].reserved_32_36 = 0;
    idt[v].gate_type = 0b110; // 0b110 - interrupt gate, 0b111 - trap gate 
    idt[v].clear_37_39 = 0; 
    idt[v].clear_44 = 0;
    idt[v].DPL = 0b0;
    idt[v].P = 0b1;          // 1 - present flag
    idt[v].offset_16_31 = ((uint32_t)(tramplins + TRAMPLIN_SIZE * v) >> 16) & 0xffff;
  }

  IDT idtr;
  idtr.limit = IDT_SIZE - 1;
  idtr.base = idt;

  lidt(&idtr);

  printf("IDT initialized\n");
}


void universal_handler(interrupt_context* context) {
  kernel_panic("unhandled interrupt #%x at %x:%x\n\n"
               "Registers: \n"
               "  EAX: %x, ECX: %x, EDX: %x, EBX: %x\n"
               "  ESP: %x, EBP: %x, ESI: %x, EDI: %x\n"
               "  DS : %x, ES : %x, FS : %x, GS : %x\n\n"
               "Error code: \n"
               "  common_error_code, value: %x\n\n"
               "EFLAGS\n"
               "  value: %x", context->int_vector, context->cs, context->eip, 
               context->eax, context->ecx, context->edx, context->ebx,
               context->esp, context->ebp, context->esi, context->edi,
               context->ds, context->es, context->fs, context->gs,
               context->error_code, context->eflags);  

}                      
                            