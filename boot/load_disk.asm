[bits 16]

; load disk sectors after the first
; params:
;   dw sector_count: how many sectors to read
;   dw dest:         destination in memory to load sectors to
;   dw drive:        drive to read sectors from
load_sectors:
    push bp
    mov bp, sp
    pusha

    mov bx, 0
    mov es, bx

    mov ax, [bp + 8]
    mov bx, [bp + 6]
    mov dx, [bp + 4]
    mov ah, 0x02
    mov ch, 0x00    ; cylinder 0
    mov dh, 0x00    ; head 0
    mov cl, 0x02    ; start from sector 2
    int 0x13

    jc disk_error

    mov dx, [bp + 8]
    cmp al, dl  ; al - sectors read
    jne disk_error

    push disk_success_msg
    call bios_println

    popa
    pop bp
    ret 4


disk_error:
    push disk_error_msg
    call bios_println

    push sectors_read_msg
    call bios_print

    mov cl, ah
    xor ah, ah
    push ax
    call bios_print_hex
    push newline
    call bios_print

    push error_code_msg
    call bios_print

    xor ch, ch
    push cx
    call bios_print_hex
    push newline
    call bios_print
    jmp short $

disk_error_msg db 'Disk read error!', 0
sectors_read_msg db 'Sectors read: ', 0
error_code_msg db 'Error code: ', 0
disk_success_msg db 'Disk read successfully!', 0