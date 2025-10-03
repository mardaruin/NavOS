#include <stdint.h>
 
#define CLEAR_SCREEN() (*((volatile uint16_t *)0xB8000) = 0)
#define SET_PINK_BACKGROUND() (*((volatile uint16_t *)0xB8000) = 0x0500)                                                           

void kernel_entry(void) {
	SET_PINK_BACKGROUND();
	while(1);
}