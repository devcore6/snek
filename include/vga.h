#ifndef __KERNEL_VGA_H
# define __KERNEL_VGA_H

# ifdef __cplusplus
extern "C" {
# endif

# include <stdint.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>

enum {
	VGA_BLACK = 0,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LGRAY,
	VGA_DGRAY,
	VGA_LBLUE,
	VGA_LGREEN,
	VGA_LCYAN,
	VGA_LRED,
	VGA_LMAGENTA,
	VGA_LBROWN,
	VGA_WHITE
};

inline uint8_t vga_color(uint8_t fg, uint8_t bg) { return fg | bg << 4; }
inline uint16_t vga_entry(char c, uint8_t color) { return (uint16_t)(unsigned char)c | (uint16_t)color << 8; }

// vga.c
extern const size_t VGA_WIDTH;
extern const size_t VGA_HEIGHT;
extern void tty_init();
extern void tty_setcolor(uint8_t fg, uint8_t bg);
extern void tty_scroll();
extern void tty_putat(char c, uint8_t x, uint8_t y);
extern void tty_putchar(char c);
extern void tty_write(const char* data);

# ifdef __cplusplus
}
# endif

#endif