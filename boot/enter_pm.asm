[bits 16]

switch_pm:
    cli

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax
    
    ; use a far jump to flush the cpu pipeline from real mode instructions
    jmp CODESEG:init_pm

[bits 32]

init_pm:
    mov eax, DATASEG
    mov ds, eax
    mov ss, eax
    mov fs, eax
    mov gs, eax
    mov es, eax

    mov ebp, 0x80000
    mov esp, ebp

    call begin_pm
