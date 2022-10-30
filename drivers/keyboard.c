#include "keyboard.h"
#include "../hardware/idt.h"
#include "../hardware/isr.h"
#include "../hardware/port.h"
#include "../hardware/pic.h"
#include "../lib/logging.h"

static bool caps_toggled = false;
static bool num_lock = false;

static bool pressed[0x100] = {0};
static bool virtual_pressed[0x100] = {0};

static int last_scan_code = 0;

static void (*dispatch)(keyboard_key_t key);

static inline bool is_virtual(keyboard_key_t key)
{
    return key > 0xff;
}

static void handle_key(keyboard_key_t key, bool is_pressed);
static void handle_scan_code(int scan_code);
static void isr(interrupt_frame *frame);

void handle_key(keyboard_key_t key, bool is_pressed)
{
    if (is_virtual(key))
        virtual_pressed[key & 0xff] = is_pressed;
    else
        pressed[key] = is_pressed;

    if (key == K_CAPS && is_pressed)
        caps_toggled = !caps_toggled;
    if (key == K_NUM && is_pressed)
        num_lock = !num_lock;

    dispatch(key);
}

void handle_scan_code(int scan_code)
{
    bool is_pressed = scan_code < 0x80;
    keyboard_key_t key = is_pressed ? scan_code : scan_code - 0x80;

    if (last_scan_code == _VIRTUAL_SCAN_CODE)
    {
        key = (last_scan_code << 8) | key;
        last_scan_code = 0;
        handle_key(key, is_pressed);
    }
    else if (last_scan_code == 0)
    {
        if (scan_code == _VIRTUAL_SCAN_CODE)
            last_scan_code = scan_code;
        else
            handle_key(key, is_pressed);
    }
    else
    {
        ERROR("unknown scan code sequence: %x and %x\n", last_scan_code, scan_code);
        last_scan_code = 0;
    }
}

__attribute__((interrupt)) void isr(interrupt_frame *frame)
{
    int i, scan_code;
    scan_code = inb(0x60);
    handle_scan_code(scan_code);

    i = inb(0x61);
    outb(0x61, i | 0x80);
    outb(0x61, i);

    echo_isr(IRQ1);
}

static char key2char[0x100] = {
    [K_0] = '0',
    [K_1] = '1',
    [K_2] = '2',
    [K_3] = '3',
    [K_4] = '4',
    [K_5] = '5',
    [K_6] = '6',
    [K_7] = '7',
    [K_8] = '8',
    [K_9] = '9',
    [K_A] = 'a',
    [K_B] = 'b',
    [K_C] = 'c',
    [K_D] = 'd',
    [K_E] = 'e',
    [K_F] = 'f',
    [K_G] = 'g',
    [K_H] = 'h',
    [K_I] = 'i',
    [K_J] = 'j',
    [K_K] = 'k',
    [K_L] = 'l',
    [K_M] = 'm',
    [K_N] = 'n',
    [K_O] = 'o',
    [K_P] = 'p',
    [K_Q] = 'q',
    [K_R] = 'r',
    [K_S] = 's',
    [K_T] = 't',
    [K_U] = 'u',
    [K_V] = 'v',
    [K_W] = 'w',
    [K_X] = 'x',
    [K_Y] = 'y',
    [K_Z] = 'z',

    [K_SINGLE_QUOTE] = '\'',
    [K_COMMA] = ',',
    [K_DASH] = '-',
    [K_DOT] = '.',
    [K_SLASH] = '/',
    [K_SEMICOLON] = ';',
    [K_EQUAL] = '=',

    [K_KP_TIMES] = '*',
    [K_KP_PLUS] = '+',
    [K_KP_MINUS] = '-',
    // [K_KP_DIV] = '/', virtual scan code!
    [K_KP_DOT] = '.',
    [K_KP_0] = '0',
    [K_KP_1] = '1',
    [K_KP_2] = '2',
    [K_KP_3] = '3',
    [K_KP_4] = '4',
    [K_KP_5] = '5',
    [K_KP_6] = '6',
    [K_KP_7] = '7',
    [K_KP_8] = '8',
    [K_KP_9] = '9',

    // [K_TAB] = '\t',

    [K_OPENING_SQUARE_BRACKET] = '[',
    [K_CLOSING_SQUARE_BRACKET] = ']',
    [K_BACKSLASH] = '\\',
    [K_BACKTICK] = '`',
    [K_SPACE] = ' ',
};

static char shift_key2char[0x100] = {
    [K_0] = ')',
    [K_1] = '!',
    [K_2] = '@',
    [K_3] = '#',
    [K_4] = '$',
    [K_5] = '%',
    [K_6] = '^',
    [K_7] = '&',
    [K_8] = '*',
    [K_9] = '(',
    [K_A] = 'A',
    [K_B] = 'B',
    [K_C] = 'C',
    [K_D] = 'D',
    [K_E] = 'E',
    [K_F] = 'F',
    [K_G] = 'G',
    [K_H] = 'H',
    [K_I] = 'I',
    [K_J] = 'J',
    [K_K] = 'K',
    [K_L] = 'L',
    [K_M] = 'M',
    [K_N] = 'N',
    [K_O] = 'O',
    [K_P] = 'P',
    [K_Q] = 'Q',
    [K_R] = 'R',
    [K_S] = 'S',
    [K_T] = 'T',
    [K_U] = 'U',
    [K_V] = 'V',
    [K_W] = 'W',
    [K_X] = 'X',
    [K_Y] = 'Y',
    [K_Z] = 'Z',

    [K_SINGLE_QUOTE] = '"',
    [K_COMMA] = '<',
    [K_DASH] = '_',
    [K_DOT] = '>',
    [K_SLASH] = '?',
    [K_SEMICOLON] = ':',
    [K_EQUAL] = '+',

    // [K_TAB] = '\t',

    [K_OPENING_SQUARE_BRACKET] = '{',
    [K_CLOSING_SQUARE_BRACKET] = '}',
    [K_BACKSLASH] = '|',
    [K_BACKTICK] = '~',
    [K_SPACE] = ' ',
};

static bool numlock_keys[0x100] = {
    [K_KP_0] = true,
    [K_KP_1] = true,
    [K_KP_2] = true,
    [K_KP_3] = true,
    [K_KP_4] = true,
    [K_KP_5] = true,
    [K_KP_6] = true,
    [K_KP_7] = true,
    [K_KP_8] = true,
    [K_KP_9] = true,
    [K_KP_DOT] = true,
};

char get_char(keyboard_key_t key, bool shift)
{
    if (is_virtual(key))
    {
        if (key == K_KP_DIV)
            return '/';
        return '\0';
    }
    if (num_lock && numlock_keys[key])
        return '\0';

    if (shift)
        return shift_key2char[key];
    return key2char[key];
}

void register_keyboard(void (*handler)(keyboard_key_t key))
{
    register_isr(IRQ1, (void *)isr);
    dispatch = handler;
}

bool is_key_pressed(keyboard_key_t key)
{
    if (is_virtual(key))
        return virtual_pressed[key & 0xff];
    return pressed[key];
}

bool is_shift_on(void)
{
    return (is_key_pressed(K_L_SHFT) || is_key_pressed(K_R_SHFT)) ^ caps_toggled;
}