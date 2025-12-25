#include "kernel_entry.h"
#include "alloc.h"
#include "assert.h"
#include "interrupts.h"
#include "memory.h"
#include "panic.h"
#include "printer.h"
#include "string.h"
#include "types.h"
#include "vga.h"
#include <stdbool.h>

#define SCREEN_HEIGHT 25
#define BLOCK_SIZE 123
#define ALIGNMENT_VALUE 16

static size_t screen_y_pos = 0;

void kernel_entry() {
  vga_clear_screen();

  fifth_lab();

  //fourth_lab();
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

  setup8259(false); // true if aeoi

  // DEVICE_MASK_NONE
  // DEVICE_MASK_TIMER
  // DEVICE_MASK_KEYBOARD
  // DEVICE_MASK_BOTH

  /* Here I try to copy conwors precision zerofication */
  bool enable = true;
  uint8_t timer_code = 0;
  uint8_t value = read_from_port(MASTER_DATA_PORT);
  //uint8_t timer_enable_mask = ((value) & ~(0x1 << (bit_number)))
  uint8_t timer_enable_mask = ((value) & ~(0x1 << (timer_code)));
  // write_to_port(MASTER_DATA_PORT, timer_enable_mask);

  /* but in the end just write in into data in your style*/
  write_to_port(MASTER_DATA_PORT, DEVICE_MASK_TIMER);

  //set_device(DEVICE_MASK_TIMER);

  // vga_clear_screen();
  // DELAY_MS(10);

  sti();
  inf_loop();
}