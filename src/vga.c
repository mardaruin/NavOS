#include "types.h"
#include "vga.h"

#define SCREEN_HEIGHT 25
#define SCREEN_WIDE 80


void vga_clear_screen() {
	volatile uint16_t* video_memory = (uint16_t*)0xB8000;
	for (int i = 0; i < SCREEN_WIDE * SCREEN_HEIGHT; ++i) {
		video_memory[i] = 0x0720; // пробел
	}
}

void vga_print_char(char c, size_t x, size_t y) {
	volatile uint16_t* video_memory = (uint16_t*)0xB8000;
	video_memory[y * SCREEN_WIDE + x] = (c | 0x0700);
	
}

void vga_scroll_down() {
	volatile uint16_t* video_memory = (uint16_t*)0xB8000;
	for (int i = 0; i < SCREEN_WIDE * (SCREEN_HEIGHT - 1); ++i) {
		video_memory[i] = video_memory[i + SCREEN_WIDE];
	}
	for (int i = SCREEN_WIDE *(SCREEN_HEIGHT - 1); i < SCREEN_WIDE * SCREEN_HEIGHT; ++i) {
		video_memory[i] = 0x0720;
	}
}