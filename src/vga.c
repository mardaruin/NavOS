#include "vga.h"
#include "memory.h"
#include "types.h"

#define SCREEN_HEIGHT 25
#define SCREEN_WIDE 80
#define SPACE 0x0720
#define VIDEO_MEMORY ((uint16_t *)0xB8000)
#define WHITE_BACKGROUND 0x0700

void vga_clear_screen() {
  volatile uint16_t *video_memory = VIDEO_MEMORY;
  for (int i = 0; i < SCREEN_WIDE * SCREEN_HEIGHT; ++i) {
    video_memory[i] = SPACE;
  }
}

void vga_print_char(char c, size_t x, size_t y) {
  volatile uint16_t *video_memory = VIDEO_MEMORY;
  video_memory[y * SCREEN_WIDE + x] = (c | WHITE_BACKGROUND);
}

void vga_scroll_down() {
  volatile uint16_t *video_memory = VIDEO_MEMORY;
  memmove(video_memory, video_memory + SCREEN_WIDE,
          sizeof(uint16_t) * (SCREEN_WIDE * (SCREEN_HEIGHT - 1)));
  for (int i = SCREEN_WIDE * (SCREEN_HEIGHT - 1);
       i < SCREEN_WIDE * SCREEN_HEIGHT; ++i) {
    video_memory[i] = SPACE;
  }
}