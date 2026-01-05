#include "kernel_entry.h"
#include "alloc.h"
#include "assert.h"
#include "interrupts.h"
#include "memory.h"
#include "panic.h"
#include "printer.h"
#include "string.h"
#include "vga.h"
#include <stdbool.h>

#define SCREEN_HEIGHT 25
#define BLOCK_SIZE 123
#define ALIGNMENT_VALUE 16
#define STACK_SIZE 4096

static size_t screen_y_pos = 0;

void fourth_lab(void);
void fifth_lab(void);
void six_lab(void);
void user_program(void);

void kernel_entry() {
  vga_clear_screen();

  six_lab();

  // fifth_lab();

  // fourth_lab();
}

void fourth_lab(void) {
  init_idt(INTERRUPT_GATE);
  setup_registers();

  // div_zero();

  // pseudo_syscall();
  sti();

  inf_loop();
}

void fifth_lab(void) {
  // INTERRUPT_GATE
  // TRAP_GATE
  init_idt(INTERRUPT_GATE);
  setup_registers();

  setup8259(true); // true if aeoi

  // DEVICE_MASK_NONE
  // DEVICE_MASK_TIMER
  // DEVICE_MASK_KEYBOARD
  // DEVICE_MASK_BOTH

  set_device(DEVICE_MASK_BOTH);

  // vga_clear_screen();
  // DELAY_MS(10);

  sti();
  inf_loop();
}

void six_lab(void) {
  init_idt(INTERRUPT_GATE);
  // setup_registers();
  setup8259(true);
  set_device(DEVICE_MASK_TIMER);
  uint8_t *stack = malloc_undead(STACK_SIZE, ALIGNMENT_VALUE) + STACK_SIZE;
  start_process(user_program, stack);
  // inf_loop();
}

void user_program(void) {
  // printf("User process\n");

  // printf("%d ", gl_counter++);

  while (true) {
    printf("%d ", inc_global_counter());
  }

  // code_segment_desc &= ~((uint64_t)1 << 47ma); // p = 0
  // inf_loop_by_syscall();

  // printf("%p ", get_esp());

  // not_allowed();

  // inf_loop();
}