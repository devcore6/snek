gdt:

gdt_null:
	dq 0

gdt_code:
	dq 0x00CF9A000000FFFF

gdt_data:
	dq 0x00CF92000000FFFF

gdt_end:

gdt_desc:

gdt_size:
	dw gdt_end - gdt - 1
	dq gdt_null
	codeseg equ gdt_code - gdt_null
	dataseg equ gdt_data - gdt_null

global load_gdt
load_gdt:
	cli
	lgdt [gdt_desc]
	jmp codeseg:reload_cs
reload_cs:
	mov ax, dataseg
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	sti
	ret
