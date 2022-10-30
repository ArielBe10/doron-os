#include "vga.h"
#include "../hardware/port.h"

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

static volatile uint16_t *const VIDEO_MEMORY = (uint16_t *)0xb8000;
int current_cursor_offset = 0;
int current_cursor_x = 0, current_cursor_y = 0;

void _raw_putc(char c, int x, int y, vga_attr attrs)
{
    if (x >= COLUMNS || y >= ROWS)
        return;
    *(VIDEO_MEMORY + y * COLUMNS + x) = c | ((*(uint8_t *)&attrs) << 8);
}

char _raw_getc(int x, int y)
{
    if (x >= COLUMNS || y >= ROWS)
        return -1;
    return *(char *)(VIDEO_MEMORY + y * COLUMNS + x);
}

vga_attr _raw_get_attr(int x, int y)
{
    return *(vga_attr *)((char *)(VIDEO_MEMORY + y * COLUMNS + x) + 1);
}

int _raw_get_cursor_offset(void)
{
    int pos = 0;
    outb(REG_SCREEN_CTRL, 0x0F);
    io_wait();
    pos |= inb(REG_SCREEN_DATA);
    io_wait();
    outb(REG_SCREEN_CTRL, 0x0E);
    io_wait();
    pos |= ((int)inb(REG_SCREEN_DATA)) << 8;
    io_wait();
    return pos;
}

void _raw_set_cursor_offset(int offset)
{
    outb(REG_SCREEN_CTRL, 0x0F);
    io_wait();
    outb(REG_SCREEN_DATA, (uint8_t)(offset & 0xFF));
    io_wait();
    outb(REG_SCREEN_CTRL, 0x0E);
    io_wait();
    outb(REG_SCREEN_DATA, (uint8_t)((offset >> 8) & 0xFF));
    io_wait();
}

void set_cursor_offset(int offset)
{
    current_cursor_offset = offset;
    current_cursor_x = to_cursor_x(offset);
    current_cursor_y = to_cursor_y(offset);
    _raw_set_cursor_offset(offset);
}