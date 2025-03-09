#include "keyboard.h"
#include "port.h"
#include "screen.h"

// US keyboard layout
static char keyboard_us[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// US keyboard layout (shift)
static char keyboard_us_shift[128] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static int shift_pressed = 0;
static int caps_lock = 0;
static int ctrl_pressed = 0;
static int alt_pressed = 0;

void keyboard_init() {
    // Setup keyboard IRQ here if we had IDT
}

char keyboard_process_key() {
    // Check if there is a key in the buffer
    if ((port_byte_in(0x64) & 1) == 0) {
        return 0;
    }
    
    unsigned char scancode = port_byte_in(0x60);
    
    // Handle key release
    if (scancode & 0x80) {
        scancode &= 0x7F; // Remove the bit that indicates key release
        
        if (scancode == KEY_LEFT_SHIFT || scancode == KEY_RIGHT_SHIFT) {
            shift_pressed = 0;
        } else if (scancode == KEY_LEFT_CTRL) {
            ctrl_pressed = 0;
        } else if (scancode == KEY_LEFT_ALT) {
            alt_pressed = 0;
        }
        
        return 0;
    }
    
    // Handle key press
    if (scancode == KEY_LEFT_SHIFT || scancode == KEY_RIGHT_SHIFT) {
        shift_pressed = 1;
        return 0;
    } else if (scancode == KEY_LEFT_CTRL) {
        ctrl_pressed = 1;
        return 0;
    } else if (scancode == KEY_LEFT_ALT) {
        alt_pressed = 1;
        return 0;
    } else if (scancode == KEY_CAPS_LOCK) {
        caps_lock = !caps_lock;
        return 0;
    }
    
    // Handle special keys
    if (scancode == KEY_BACKSPACE) {
        backspace();
        return '\b';
    } else if (scancode == KEY_UP_ARROW || scancode == KEY_DOWN_ARROW ||
               scancode == KEY_LEFT_ARROW || scancode == KEY_RIGHT_ARROW) {
        handle_special_key(scancode);
        return 0;
    }
    
    // Get the character
    char c;
    if (shift_pressed) {
        c = keyboard_us_shift[scancode];
    } else {
        c = keyboard_us[scancode];
    }
    
    // Apply caps lock for letters
    if (caps_lock && c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    } else if (caps_lock && c >= 'A' && c <= 'Z') {
        c = c - 'A' + 'a';
    }
    
    // If it's a printable character
    if (c) {
        if (c == '\n') {
            print_at_cursor("\n");
        } else {
            char str[2] = {c, '\0'};
            print_at_cursor(str);
        }
    }
    
    return c;
}

int keyboard_get_line(char* buffer, int max_length) {
    int i = 0;
    char c;
    
    while (i < max_length - 1) {
        c = 0;
        
        // Wait for a key
        while (c == 0) {
            c = keyboard_process_key();
        }
        
        // Enter key - end of input
        if (c == '\n') {
            buffer[i] = '\0';
            return i;
        }
        
        // Backspace - remove last character
        if (c == '\b') {
            if (i > 0) {
                i--;
            }
            continue;
        }
        
        // Add the character to the buffer
        if (c != 0) {
            buffer[i] = c;
            i++;
        }
    }
    
    buffer[i] = '\0';
    return i;
}