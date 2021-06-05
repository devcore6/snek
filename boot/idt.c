#include <stdint.h>
#include <io.h>
#include <kb.h>

extern void irq0();

struct IDT_entry{
	unsigned short int offset_lowerbits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int offset_higherbits;
};

struct interrupt_frame {
	uint16_t ip;
	uint16_t cs;
	uint16_t flags;
	uint16_t sp;
	uint16_t ss;
} __attribute__((packed));

struct IDT_entry IDT[256];

bool _w = false;
bool _a = false;
bool _s = false;
bool _d = false;
bool _enter = false;

extern uint8_t snake_direction;
extern bool dead;

__attribute__((interrupt)) void irq1(struct interrupt_frame* frame) {
	if(inb(0x64) & 0x01) {
		uint8_t ichar = inb(0x60);
		switch(ichar) {
			case PS2_W: { _w = true; break; }
			case PS2_A: { _a = true; break; }
			case PS2_S: { _s = true; break; }
			case PS2_D: { _d = true; break; }
			case PS2_ENTER: { _enter = true; break; }
			case PS2_RELEASE_W: { _w = false; break; }
			case PS2_RELEASE_A: { _a = false; break; }
			case PS2_RELEASE_S: { _s = false; break; }
			case PS2_RELEASE_D: { _d = false; break; }
			case PS2_RELEASE_ENTER: { _enter = false; break; }
		}
	}

	// Update snake direction
	if(!dead) {
		if(_w && snake_direction != 2) snake_direction = 0;
		if(_a && snake_direction != 3) snake_direction = 1;
		if(_s && snake_direction != 0) snake_direction = 2;
		if(_d && snake_direction != 1) snake_direction = 3;
	}

	outb(0x20, 0x20);	// Tell PIC we are ready to receive more interrupts
}

void idt_init() {
	unsigned long irq0_address; // PIT
	unsigned long irq1_address; // PS2

	// Remap PIC
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 40);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	irq0_address = (unsigned long)irq0;
	IDT[32].offset_lowerbits = irq0_address & 0xFFFF;
	IDT[32].selector = 0x08;
	IDT[32].zero = 0;
	IDT[32].type_attr = 0x8e;
	IDT[32].offset_higherbits = (irq0_address & 0xFFFF0000) >> 16;

	irq1_address = (unsigned long)irq1;
	IDT[33].offset_lowerbits = irq1_address & 0xFFFF;
	IDT[33].selector = 0x08;
	IDT[33].zero = 0;
	IDT[33].type_attr = 0x8e;
	IDT[33].offset_higherbits = (irq1_address & 0xFFFF0000) >> 16;

	unsigned long idt_address = (unsigned long)IDT;
	unsigned long idt_ptr[2] = {
		(sizeof(struct IDT_entry) * 256) + ((idt_address & 0xFFFF) << 16),
		idt_address >> 16
	};

	asm volatile("lidt %0" : : "m"(idt_ptr));
}