#include "vga.h"

#include "types.h"

#define SCREEN_HEIGHT 25
#define SCREEN_WIDE 80
#define SPACE 0x0720
#define VIDEO_MEMORY 0xB8000
#define WHITE_BACKGROUND 0x0700

void vga_clear_screen() {
  volatile uint16_t* video_memory = (uint16_t*) VIDEO_MEMORY;
  for (int i = 0; i < SCREEN_WIDE * SCREEN_HEIGHT; ++i) {
    video_memory[i] = SPACE;
  }
}

void vga_print_char(char c, size_t x, size_t y) {
  volatile uint16_t* video_memory   = (uint16_t*) VIDEO_MEMORY;
  video_memory[y * SCREEN_WIDE + x] = (c | WHITE_BACKGROUND);
}

void vga_scroll_down() {
  volatile uint16_t* video_memory = (uint16_t*) VIDEO_MEMORY;
  for (int i = 0; i < SCREEN_WIDE * (SCREEN_HEIGHT - 1); ++i) {
    video_memory[i] = video_memory[i + SCREEN_WIDE];
  }
  for (int i = SCREEN_WIDE * (SCREEN_HEIGHT - 1); i < SCREEN_WIDE * SCREEN_HEIGHT; ++i) {
    video_memory[i] = 0x0720;
  }
}
