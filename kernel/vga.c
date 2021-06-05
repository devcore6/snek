#include <vga.h>

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;
uint16_t* vga_memory = (void*)0xB8000;

uint8_t tty_x;
uint8_t tty_y;
uint16_t tty_color;

void tty_init() {
	tty_x = 0;
	tty_y = 0;
	tty_color = vga_color(VGA_WHITE, VGA_BLACK);
	uint16_t clear_entry = vga_entry(' ', tty_color);
	for(size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga_memory[i] = clear_entry;
}

void tty_setcolor(uint8_t fg, uint8_t bg) {
	tty_color = vga_color(fg, bg);
}

void tty_scroll() {
	if(++tty_y == VGA_HEIGHT) {
		tty_y--;
		// Scroll terminal up
		for(size_t i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++)
			vga_memory[i] = vga_memory[i + VGA_WIDTH];
		uint16_t clear_entry = vga_entry(' ', tty_color);
		for(size_t i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++)
			vga_memory[i] = clear_entry;
	}
}

void tty_putat(char c, uint8_t x, uint8_t y) {
	vga_memory[y * VGA_WIDTH + x] = vga_entry(c, tty_color);
}

void tty_putchar(char c) {
	if(c == '\n') {	// Line break on \n
		tty_scroll();
		return;
	}

	vga_memory[tty_y * VGA_WIDTH + tty_x] = vga_entry(c, tty_color);
	if(++tty_x == VGA_WIDTH) {
		tty_x = 0;
		tty_scroll();
	}
}

void tty_write(const char* data) {
	size_t len = strlen(data);
	for(size_t i = 0; i < len; i++)
		tty_putchar(data[i]);
}
