[org 0x7c00]    ; bootloader code starts at 0x7c00
[bits 16]

KERNEL_OFFSET equ 0x1000

boot_start:
    ; create stack
    mov bp, 0x8000
    mov sp, bp

    ; save boot drive from dl set by bios
    xor dh, dh
    mov [boot_drive], dx

    push real_mode_msg
    call bios_println

    ; load additional sectors
    push 54
    push KERNEL_OFFSET
    push word [boot_drive]
    call load_sectors

    call KERNEL_OFFSET

%include "boot/bios_print.asm"
%include "boot/load_disk.asm"

; data
real_mode_msg db 'Started in real mode!', 0
boot_drive dw 0

times 510-($-$$) db 0   ; padding
dw 0xaa55   ; magic
