[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; print a null terminated ascii message in protected mode
; params:
;   dd message: the address of the message to be printed
pm_print:
    push ebp
    mov ebp, esp
    pusha

    mov ebx, [ebp + 8]

    mov esi, VIDEO_MEMORY
    mov edi, 0x00
    mov ah, WHITE_ON_BLACK

.loop:
    mov al, [ebx + edi]
    cmp al, 0x00
    je .end
    mov [esi + 2 * edi], ax
    inc edi
    jmp .loop

.end:
    popa
    pop ebp

    ret 4
