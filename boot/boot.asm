; Multiboot header stuff
MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
FLAGS equ MBALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .bss
align 16
stack_bottom:
	resb 16384			; 16KB reserved for initial stack
stack_top:

section .text
global _loader:function (_loader.end - _loader)
_loader:
	mov esp, stack_top	; Set ESP to stack top (stack on x86 grows downwards)

	extern load_gdt
	call load_gdt

	extern idt_init
	call idt_init

	mov ebx, 1
	extern init_pit
	call init_pit

	extern kernel_main
	call kernel_main

	cli
.hang:	hlt				; This should never be called as kernel should never return
		jmp .hang
.end:
