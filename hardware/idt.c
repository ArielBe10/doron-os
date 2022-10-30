#include "idt.h"
#include <stdint.h>

#define KERNEL_CS 8
#define IDT_ENTRIES 256

typedef struct
{
    uint16_t isr_low;   // the lower 16 bits of the ISR's address
    uint16_t kernel_cs; // the GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t reserved;   // set to zero
    uint8_t attributes; // type and attributes
    uint16_t isr_high;  // the higher 16 bits of the ISR's address
} __attribute__((packed)) idt_gate;

typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr;

__attribute__((aligned(0x10))) idt_gate idt[IDT_ENTRIES];

idtr idt_reg;

void register_isr(int int_n, void *isr)
{
    uint32_t handler = (uint32_t)isr;
    idt[int_n].isr_low = handler & 0xffff;
    idt[int_n].isr_high = handler >> 16;
    idt[int_n].reserved = 0;
    idt[int_n].kernel_cs = KERNEL_CS;
    idt[int_n].attributes = 0x8e; // type 32 bit interrupt
}

void load_idtr(void)
{
    idt_reg.base = (uint32_t)idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate) - 1;
    __asm__ volatile("lidt %0" ::"m"(idt_reg));
    __asm__ volatile("sti");
}
