#pragma once

#include "../drivers/vga.h"

void init_screen(void);

void putc(char c);
void print(const char *string);
void println(const char *string);
void print_at(const char *string, int x, int y);
void printf(const char *fmt, ...);

void set_attrs(vga_attr attrs);
vga_attr get_attrs(void);

void erase(void);
void clear_line(int line);
void clear(void);

void scroll(void);
void newline(void);
