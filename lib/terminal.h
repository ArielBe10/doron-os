#pragma once

typedef enum
{
    SINGLE_LINE,
    MULTILINE,
    EXPLICIT
} input_mode_t;

void init_terminal(void);
void input(char *dst_input_str, input_mode_t in_mode);
