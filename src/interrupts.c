#include "interrupts.h"
#include "types.h"
#include "vga.h"

#define DELAY_MS(ms)                                                           \
  ({                                                                           \
    volatile uint32_t delay = ms * 100000;                                     \
    while (delay--) {                                                          \
    }                                                                          \
  })

static bool has_error_code(uint8_t v) {
  switch (v) {
  case 0x8:  // Double Fault
  case 0xA:  // Invalid TSS
  case 0xB:  // Segment Not Present
  case 0xC:  // Stack-Segment Fault
  case 0xD:  // General Protection Fault
  case 0xE:  // Page Fault
  case 0x11: // Alignment Check
  case 0x15:
  case 0x1D:
  case 0x1E:
    return true;
  default:
    return false;
  }
}

void init_idt(GATE_TYPE gate_type) {
  uint8_t *tramplins =
      (uint8_t *)malloc_undead(TRAMPLIN_SIZE * VECTORS_AMOUNT, 1);

  uint32_t v;
  for (v = 0; v < VECTORS_AMOUNT; v++) {

    bool v_has_error_code = has_error_code(v);

    uint8_t *tramplin = (uint8_t *)(tramplins + v * TRAMPLIN_SIZE);
    uint32_t offset = 0;

    void *bridge_handler =
        v_has_error_code ? collect_context_without_error_code : collect_context;

    tramplin[offset++] = 0x6a; // push imm8
    tramplin[offset++] = v;
    tramplin[offset++] = 0xe9; // jmp

    uint32_t jmp_offset =
        (uint32_t)bridge_handler - (uint32_t)(tramplin + offset + 4);
    *(uint32_t *)(tramplin + offset) =
        jmp_offset; // collect_context relative address

    offset += 4;
  }

  idt_descriptor *idt = malloc_undead(IDT_SIZE, IDT_ALIGNMENT);
  for (uint16_t v = 0; v < VECTORS_AMOUNT; v++) {
    idt[v].offset_0_15 = (uint32_t)(tramplins + TRAMPLIN_SIZE * v) & 0xffff;
    idt[v].segment_selector = 0x08;
    idt[v].reserved_32_36 = 0;
    idt[v].gate_type = gate_type; // 0b110 - interrupt gate, 0b111 - trap gate
    idt[v].clear_37_39 = 0;
    idt[v].clear_44 = 0;
    idt[v].DPL = 0b0;
    idt[v].P = 0b1; // 1 - present flag
    idt[v].offset_16_31 = (uint32_t)(tramplins + TRAMPLIN_SIZE * v) >> 16;
  }

  IDT idtr;
  idtr.limit = IDT_SIZE - 1;
  idtr.base = idt;
  lidt(&idtr);
}

void setup8259(bool aeoi) {
  // printf("In setup8259\n");
  write_to_port(MASTER_DATA_PORT, 0xff);
  write_to_port(SLAVE_DATA_PORT, 0xff);

  // DELAY_MS(10);

  // printf("ICW1:\n");
  write_to_port(MASTER_COMMAND_PORT, ICW1_CASCADE_MODE);
  write_to_port(SLAVE_COMMAND_PORT, ICW1_CASCADE_MODE);

  // DELAY_MS(10);

  // printf("ICW1 done\n");
  // printf("ICW2:\n");
  write_to_port(MASTER_DATA_PORT, ICW2_MASTER);
  write_to_port(SLAVE_DATA_PORT, ICW2_SLAVE);

  // DELAY_MS(10);

  // printf("ICW2 done\n");
  // printf("ICW3:\n");
  write_to_port(MASTER_DATA_PORT, ICW3_MASTER);
  write_to_port(SLAVE_DATA_PORT, ICW3_SLAVE);

  // DELAY_MS(10);

  // printf("ICW3 done\n");
  // printf("ICW4:\n");
  write_to_port(MASTER_DATA_PORT,
                ((uint8_t)aeoi << 1 | ICW4_FULLY_NESTED_MODE));
  write_to_port(SLAVE_DATA_PORT, ((uint8_t)aeoi << 1 | ICW4_FULLY_NESTED_MODE));

  // printf("ICW4 done\n");

  // DELAY_MS(10);

  write_to_port(MASTER_DATA_PORT, ~(1 << SLAVE_IRQ_IN_MASTER));
  write_to_port(SLAVE_DATA_PORT, 0xff);
  // printf("End of setup\n");
}

void send_eoi() { write_to_port(MASTER_COMMAND_PORT, EOI); }

void change_device_status(PIC_DEVICES device, bool enable) {
  short port = MASTER_DATA_PORT;

  uint8_t val = read_from_port(port);
  if (enable) {
    val = __clear_bit(val, device);
  } else {
    val = __set_bit(val, device);
  }
}

void disable_device(int device) { change_device_status(device, false); }

int global_counter = 0;

void keyboard_handler(interrupt_context *context) {
  // for experiments
  // print_panic(context);

  uint8_t byte = read_from_port(CONTROLLER_REGISTER);
  printf("%d ", byte);
  // sti();
  // while (true) {
  //   // printf("%d ", global_counter++);
  // }
  sti();
  // send_eoi();
  inf_loop();
  return;
}

static void delay() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 50000; j++) {
      write_to_port(0x80, 0x80);
    }
    printf("%d ", i);
  }
  printf("\n");
}

void timer_handler(interrupt_context *context) {
  // for experiments
  // print_panic(context);

  // universal_handler(struct interrupt_context * context);

  // printf("%d ", global_counter++);
  //    global_counter = 0;
  //   disable_device(DEVICE_MASK_TIMER);

  // delay();
  // sti();
  // delay();

  // if (global_counter < N) {
  //   send_eoi();
  //   // sti();
  // }

  // while (true) {
  //   // printf("%d ", global_counter++);
  // }

  // send_eoi();

  // global_counter = 0;
  //  printf("%d ", global_counter++);

  // printf("%d ", 0);

  printf("%p ", get_esp());

  sti();
  // inf_loop();

  return;
}

void set_device(uint8_t device) { write_to_port(MASTER_DATA_PORT, device); }

void inf_loop_with_inc() {
  for (;;) {
    printf("%d ", global_counter++);
  }
}

static const char *error_type_message(uint8_t v) {
  if (has_error_code(v)) {
    return "common_error_code";
  }
  return "fake_error_code";
}

void print_panic(interrupt_context *context) {
  kernel_panic("Struct address: %p\n"
               "Unhandled interrupt #%x at %p:%p\n\n"
               "Registers: \n"
               "  EAX: %p, ECX: %p, EDX: %p, EBX: %p\n"
               "  ESP: %p, EBP: %p, ESI: %p, EDI: %p\n"
               "  DS : %p, ES : %p, FS : %p, GS : %p\n\n"
               "Error code: \n"
               "  %s, value: %p\n\n"
               "EFLAGS\n"
               "  value: %p\n",
               context, context->int_vector, context->cs, context->eip,
               context->eax, context->ecx, context->edx, context->ebx,
               context->esp, context->ebp, context->esi, context->edi,
               context->ds, context->es, context->fs, context->gs,
               error_type_message(context->int_vector), context->error_code,
               context->eflags);
}

void gp_handler(interrupt_context *context) {
  print_panic(context);
  inf_loop();
}

void start_process(void *user_program, void *stack) {
  user_context us_context;
  us_context.context.cs = 0x1b; // ind = 11, gdt, pl=11
  us_context.context.ds = 0x23; // ind = 100, gdt, pl=11
  us_context.context.es = 0x23; // ind = 100, gdt, pl=11
  us_context.context.fs = 0x23; // ind = 100, gdt, pl=11
  us_context.context.gs = 0x23; // ind = 100, gdt, pl=11
  us_context.context.eip = (uint32_t)user_program;
  us_context.esp = (uint32_t)stack;
  us_context.context.eflags = ((eflags() & ~(0b11 << 12)) | (1 << 9));
  us_context.ss = 0x23; // ind = 100, gdt, pl=11
  // printf("%x ", us_context.context.eflags);
  // inf_loop();
  restore_context(&us_context);
}

void universal_handler(interrupt_context *context) {
  switch (context->int_vector) {
  case 0x20:
    // printf("Timer device\n");
    timer_handler(context);
    break;
  case 0x21:
    // printf("Keyboard device\n");
    keyboard_handler(context);
    break;
  case 0x13:
    gp_handler(context);
    break;
  default:
    printf("Default handler\n");
    print_panic(context);
    break;
  }
}
