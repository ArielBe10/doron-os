#pragma once

#include <stdint.h>
#include "../lib/logging.h"
#include "port.h"
#include "pic.h"

static inline void silent_isr(uint8_t int_n)
{
    uint8_t irq = int_n - 0x20;

    send_eoi(irq);
    io_wait();
}

static inline void echo_isr(uint8_t int_n)
{
    DEBUG("interrupt: %d", int_n);
    int irq = int_n - 0x20;
    send_eoi(irq);
    io_wait();
}

static inline void exception_isr(uint8_t int_n, uint32_t error_code)
{
    PANIC("exception interrupt: %d, error code: %d", int_n, error_code);
}

void init_idt(void);
