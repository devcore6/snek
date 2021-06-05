CFLAGS=-O2 -g -ffreestanding -Wall -Iinclude -nostdlib -std=gnu11 # asm isn't allowed in c2x I guess
CPPFLAGS=-O2 -g -ffreestanding -Wall -Iinclude -nostdlib -fno-exceptions -fno-rtti -fno-use-cxa-atexit -std=c++2a

CC=i686-elf-gcc
CXX=i686-elf-g++

OBJECTS=\
libc/string/memcpy.o \
libc/string/memmove.o \
libc/string/memset.o \
libc/string/strcpy.o \
libc/string/strncpy.o \
libc/string/strlen.o \
libc/stdlib/rand.o \
libc/stdlib/srand.o \
boot/boot.o \
boot/idt.o \
boot/pit.o \
boot/gdt.o \
kernel/vga.o \
kernel/kernel.o \

all: $(OBJECTS)
	$(CXX) $(CPPFLAGS) -T linker.ld -o snek_kernel $(OBJECTS)
	grub-file --is-x86-multiboot snek_kernel
	mkdir -p isodir/boot/grub
	mv snek_kernel isodir/boot/snek_kernel
	echo "menuentry \"snek\" {" > isodir/boot/grub/grub.cfg
	echo "	multiboot /boot/snek_kernel" >> isodir/boot/grub/grub.cfg
	echo "}" >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o snek.iso isodir

boot/idt.o:
	$(CC) $(CFLAGS) -o boot/idt.o -c boot/idt.c -mgeneral-regs-only

%.o: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

%.o: %.asm
	nasm -f elf32 $< -o $@

clean:
	rm -f snek_kernel
	rm -f snek.iso
	rm -f $(OBJECTS)
	rm -rf isodir
