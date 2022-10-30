#pragma once

#define ROWS 25
#define COLUMNS 80

typedef enum
{
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PURPLE,
    BROWN,
    GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_PURPLE,
    YELLOW,
    WHITE
} color;

typedef struct _vga_attr
{
    color foreground : 4;
    color background : 4;
} vga_attr;

void _raw_putc(char c, int x, int y, vga_attr attrs);
char _raw_getc(int x, int y);
vga_attr _raw_get_attr(int x, int y);
void _raw_set_cursor_offset(int offset);
int _raw_get_cursor_offset(void);

void set_cursor_offset(int offset);

static inline int to_cursor_x(int offset) { return offset % COLUMNS; }
static inline int to_cursor_y(int offset) { return offset / COLUMNS; }
static inline int to_cursor_offset(int x, int y) { return y * COLUMNS + x; }
static inline void set_cursor(int x, int y) { set_cursor_offset(to_cursor_offset(x, y)); }

extern int current_cursor_offset;
extern int current_cursor_x, current_cursor_y;