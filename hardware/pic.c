#include "pic.h"

#include "port.h"

#define PIC1 0x20 /* IO base address for master PIC */
#define PIC2 0xA0 /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PIC_EOI 0x20 /* end of interrupt command code */

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4 0x01      /* ICW4 (not) needed */
#define ICW1_SINGLE 0x02    /* single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* call address interval 4 (8) */
#define ICW1_LEVEL 0x08     /* level triggered (edge) mode */
#define ICW1_INIT 0x10      /* initialization - required! */

#define ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02       /* auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08  /* buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* buffered mode/master */
#define ICW4_SFNM 0x10       /* special fully nested (not) */

#define NEW_MASTER_OFFSET 0x20
#define NEW_SLAVE_OFFSET 0x28

void send_eoi(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI);
    outb(PIC1_COMMAND, PIC_EOI);
}

void remap_pic(void)
{
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA); // save masks
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // starts the initialization sequence (in cascade mode)
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC1_DATA, NEW_MASTER_OFFSET); // ICW2: master PIC vector offset
    io_wait();
    outb(PIC2_DATA, NEW_SLAVE_OFFSET); // ICW2: slave PIC vector offset
    io_wait();
    outb(PIC1_DATA, 4); // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2); // ICW3: tell Slave PIC its cascade identity (0000 0010)
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1); // restore saved masks
    outb(PIC2_DATA, a2);
}