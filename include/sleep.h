#ifndef __SLEEP_H
# define __SLEEP_H
# include <stdint.h>
# include <vga.h>
extern volatile uint32_t system_timer_ms; // Must be volatile so GCC doesn't optimize it out

void sleep(uint32_t ms) {
	uint32_t targetms = system_timer_ms + ms;
	while(targetms > system_timer_ms) asm volatile("hlt"); // Using hlt instead of nop to put CPU in a lower power state
}

#endif
