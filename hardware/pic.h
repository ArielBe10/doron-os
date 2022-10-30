#pragma once

#include <stdint.h>

void send_eoi(uint8_t irq);
void remap_pic(void);
