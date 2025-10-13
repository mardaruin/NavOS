#include "types.h"
#include "vga.h"

void vga_clear_screen() {
	volatile uint16_t* video_memory = (uint16_t*)0xB8000;
	for (int i = 0; i < 80 * 25; ++i) {
		video_memory[i] = 0x0720; // пробел
	}
}

void vga_print_char(char c, size_t x, size_t y) {
	volatile uint16_t* video_memory = (uint16_t*)0xB8000;
	video_memory[y * 80 + x] = (c | 0x0700);
	
}

void vga_scroll_down() {
	volatile uint16_t* video_memory = (uint16_t*)0xB8000;
	for (int i = 0; i < 80 * 24; ++i) {
		video_memory[i] = video_memory[i + 80];
	}
	for (int i = 80 * 24; i < 80 * 25; ++i) {
		video_memory[i] = 0x0720;
	}
}