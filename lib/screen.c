#include "screen.h"
#include <stdarg.h>
#include "print.h"

#define MISSING_CHAR ' '

static vga_attr gattrs = {.background = BLACK,
                          .foreground = WHITE};

void init_screen(void)
{
    clear();
    set_cursor_offset(0);
}

void clear_line(int line)
{
    if (line >= ROWS)
        return;
    for (int x = 0; x < COLUMNS; x++)
    {
        _raw_putc(MISSING_CHAR, x, line, gattrs);
    }
}

void clear(void)
{
    for (int line = 0; line < ROWS; line++)
    {
        clear_line(line);
    }
}

void scroll(void)
{
    for (int y = 0; y < ROWS - 1; y++)
    {
        for (int x = 0; x < COLUMNS; x++)
        {
            _raw_putc(_raw_getc(x, y + 1), x, y, _raw_get_attr(x, y + 1));
        }
    }
    clear_line(ROWS - 1);
}

void newline(void)
{
    int gy = current_cursor_y;
    gy++;
    if (gy == ROWS)
    {
        scroll();
        gy--;
    }
    set_cursor(0, gy);
}

void putc(char c)
{
    if (c == '\n')
    {
        newline();
        return;
    }

    int gx = current_cursor_x, gy = current_cursor_y;

    _raw_putc(c, gx, gy, gattrs);
    gx++;
    if (gx == COLUMNS)
    {
        newline();
    }
    else
    {
        set_cursor(gx, gy);
    }
}

void print(const char *string)
{
    for (; *string != '\0'; string++)
    {
        putc(*string);
    }
}

void println(const char *string)
{
    print(string);
    newline();
}

void print_at(const char *string, int x, int y)
{
    for (; *string != '\0'; string++)
    {
        _raw_putc(*string, x, y, gattrs);
        x++;
        if (x == COLUMNS)
        {
            x = 0;
            y++;
            if (y == ROWS)
                return;
        }
    }
}

void set_attrs(vga_attr attrs)
{
    gattrs = attrs;
}

vga_attr get_attrs(void)
{
    return gattrs;
}

void erase(void)
{
    int gx = current_cursor_x, gy = current_cursor_y;

    if (gx == 0)
    {
        gy--;
        gx = COLUMNS - 1;
    }
    else
    {
        gx--;
    }
    _raw_putc(MISSING_CHAR, gx, gy, gattrs);
    set_cursor(gx, gy);
}

void printf(const char *fmt, ...)
{
    char str[0x100];

    va_list argp;
    va_start(argp, fmt);
    vsprintf(str, fmt, argp);
    va_end(argp);

    print(str);
}