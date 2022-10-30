#include "terminal.h"

#include "../drivers/vga.h"
#include "screen.h"
#include "print.h"
#include "string.h"
#include "../drivers/keyboard.h"

#define END_INPUT_MSG "[END]"

static char input_str[COLUMNS * ROWS];

static int input_length = 0;
static int input_index = 0;
static int input_cursor_starting_offset = 0;

static bool insert_mode = false;

static input_mode_t input_mode = false;

static bool wait_for_input = false;

static vga_attr input_attrs = {.background = BLACK, .foreground = WHITE},
                output_attrs = {.background = BLACK, .foreground = GRAY};

static void keyboard_dispatch(keyboard_key_t key);
static int find_cursor_offset(int index);
static int cursor_forward(int index, int offset);
static void erase_input(void);

void input(char *dst_input_str, input_mode_t in_mode)
{
    set_attrs(input_attrs);

    input_mode = in_mode;
    input_cursor_starting_offset = current_cursor_offset;
    input_index = 0;
    input_length = 0;
    input_str[0] = '\0';

    wait_for_input = true;
    while (wait_for_input)
    {
    }

    strcpy(dst_input_str, input_str);
    dst_input_str[input_length - 1] = '\0';

    set_attrs(output_attrs);
}

void init_terminal(void)
{
    init_screen();
    register_keyboard(keyboard_dispatch);
    set_attrs(output_attrs);
}

void handle_char(char c)
{
    if (input_index == input_length) // appending to the input
    {
        input_str[input_index] = c;
        input_str[input_index + 1] = '\0';
        input_length++;
        putc(c);
    }
    else
    {
        erase_input();
        if (insert_mode && (input_str[input_index] != '\n'))
            input_str[input_index] = c;
        else
        {
            str_insert(input_str, input_index, c);
            input_length++;
        }
        print(input_str);
    }
    input_index++;

    while (find_cursor_offset(input_length) >= ROWS * COLUMNS) // scroll
        input_cursor_starting_offset -= COLUMNS;
}

void keyboard_dispatch(keyboard_key_t key)
{
    if (!(is_key_pressed(key) && wait_for_input))
        return;
    if (key == K_D && (is_key_pressed(K_L_CTRL) || is_key_pressed(K_R_CTRL)))
    {
        input_index = input_length;
        set_cursor_offset(find_cursor_offset(input_index));
        println(END_INPUT_MSG);
        input_length++;
        input_str[input_length - 1] = '\n';
        input_str[input_length] = '\0';
        wait_for_input = false;
        return;
    }

    char c = get_char(key, is_shift_on());
    if (c)
        handle_char(c);
    else if (key == K_BKSP)
    {
        if (input_length != 0)
        {
            erase_input();
            str_remove(input_str, input_index - 1);
            print(input_str);
            input_length--;
            input_index--;
        }
    }
    else if (key == K_ENTER || key == K_KP_EN)
    {

        if (input_mode == SINGLE_LINE)
        {
            input_index = input_length;
            set_cursor_offset(find_cursor_offset(input_index));
        }
        handle_char('\n');

        if ((input_mode == SINGLE_LINE) ||
            (input_mode == MULTILINE && input_str[input_length - 2] == '\n'))
        {
            wait_for_input = false;
            return;
        }
    }
    else if (key == K_INSERT)
    {
        insert_mode = !insert_mode;
    }
    else
    {
        if (key == K_L_ARROW)
        {
            input_index = input_index == 0 ? 0 : input_index - 1;
        }
        else if (key == K_R_ARROW)
        {
            input_index = input_index == input_length ? input_length : input_index + 1;
        }
        else if (key == K_U_ARROW)
        {
            int offset = find_cursor_offset(input_index);
            while ((input_index > 0) &&
                   (to_cursor_x(offset) > current_cursor_x || to_cursor_y(offset) == current_cursor_y))
            {
                input_index--;
                offset = find_cursor_offset(input_index);
            }
        }
        else if (key == K_D_ARROW)
        {
            int offset = find_cursor_offset(input_index);
            while ((input_index < input_length) &&
                   (to_cursor_x(offset) < current_cursor_x || to_cursor_y(offset) == current_cursor_y))
            {
                input_index++;
                offset = find_cursor_offset(input_index);
            }
        }
        else if (key == K_HOME)
        {
            while (input_index > 0 && input_str[input_index - 1] != '\n')
                input_index--;
        }
        else if (key == K_END)
            while (input_str[input_index] != '\0' && input_str[input_index] != '\n')
                input_index++;
    }
    set_cursor_offset(find_cursor_offset(input_index));
}

int find_cursor_offset(int index)
{
    int offset = input_cursor_starting_offset;
    for (int i = 0; i < index; i++)
    {
        offset = cursor_forward(i, offset);
    }
    return offset;
}

int cursor_forward(int index, int offset)
{
    if (input_str[index] == '\n')
        return to_cursor_offset(0, to_cursor_y(offset) + 1);
    return offset + 1;
}

void erase_input(void)
{
    int offset = find_cursor_offset(input_length);
    set_cursor_offset(offset);
    for (int i = 0; i < offset - input_cursor_starting_offset; i++)
    {
        erase();
    }
}