gdt_start:

gdt_null:
    dd 0x00
    dd 0x00

gdt_code:
    dw 0xffff ; limit 15:0
    dw 0x00 ; base 15:0
    
    db 0x00 ; base 23:16
    db 10011010b ; P, DPL, S, type
    db 11001111b ; G, D/B, L, AVL, limit 19:16
    db 0x00 ; base 31:24

gdt_data:
    dw 0xffff ; limit 15:0
    dw 0x00 ; base 15:0

    db 0x00 ; base 23:16
    db 10010010b ; P, DPL, S, type
    db 11001111b ; G, D/B, L, AVL, limit 19:16
    db 0x00 ; base 31:24

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODESEG equ gdt_code - gdt_start
DATASEG equ gdt_data - gdt_start