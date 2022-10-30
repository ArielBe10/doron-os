[bits 16]
[extern kernel_main]

push kernel_entry_msg
call bios_println

call bios_mmap

call switch_pm

[bits 32]
begin_pm:
    push protected_mode_msg
    call pm_print

    call kernel_main
    jmp short $

%include "boot/bios_print.asm"
%include "boot/mmap.asm"

%include "boot/pm_print.asm"
%include "boot/gdt.asm"
%include "boot/enter_pm.asm"

kernel_entry_msg db 'Hello from kernel entry!', 0
protected_mode_msg db 'Switched to protected mode!', 0
