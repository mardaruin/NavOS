#include <stdint.h>
                                                                         
void clear_screen() {
    volatile uint16_t* video_memory = (uint16_t*)0xB8000;
    
    for(int i = 0; i < 25; ++i) {
        video_memory[4 * i] = (' ');
    }
}

void kernel_entry(void) {
    clear_screen();
    while(1);
}