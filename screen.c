#include "screen.h"
#include "port.h" // Will define for port I/O

// Global cursor position
int cursor_x = 0;
int cursor_y = 0;

// Screen buffer
volatile char* video_memory = (volatile char*)0xB8000;

// Clear the screen
void clear_screen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            putchar(' ', x, y, COLOR_LIGHT_GRAY);
        }
    }
    
    set_cursor_position(0, 0);
}

// Put a character at specific position with attributes
void putchar(char c, int x, int y, char attribute) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        int offset = (y * SCREEN_WIDTH + x) * 2;
        video_memory[offset] = c;
        video_memory[offset + 1] = attribute;
    }
}

// Print a string at specific position with attributes
void print(const char* str, int x, int y, char attribute) {
    int i = 0;
    while (str[i] != '\0') {
        if (x >= SCREEN_WIDTH) {
            x = 0;
            y++;
        }
        
        if (y >= SCREEN_HEIGHT) {
            scroll_screen();
            y = SCREEN_HEIGHT - 1;
        }
        
        if (str[i] == '\n') {
            x = 0;
            y++;
        } else {
            putchar(str[i], x, y, attribute);
            x++;
        }
        
        i++;
    }
}

// Print a string at current cursor position
void print_at_cursor(const char* str) {
    print(str, cursor_x, cursor_y, COLOR_LIGHT_GRAY);
    
    // Update cursor position after printing
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else {
            cursor_x++;
            if (cursor_x >= SCREEN_WIDTH) {
                cursor_x = 0;
                cursor_y++;
            }
        }
        
        if (cursor_y >= SCREEN_HEIGHT) {
            scroll_screen();
            cursor_y = SCREEN_HEIGHT - 1;
        }
        
        i++;
    }
    
    update_cursor();
}

// Handle backspace key
void backspace() {
    if (cursor_x > 0) {
        cursor_x--;
        putchar(' ', cursor_x, cursor_y, COLOR_LIGHT_GRAY);
    } else if (cursor_y > 0) {
        cursor_x = SCREEN_WIDTH - 1;
        cursor_y--;
        putchar(' ', cursor_x, cursor_y, COLOR_LIGHT_GRAY);
    }
    
    update_cursor();
}

// Initialize the cursor
void init_cursor() {
    // Set cursor size (scanlines 0-15)
    port_byte_out(0x3D4, 0x0A);
    port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xC0) | 0);
    
    port_byte_out(0x3D4, 0x0B);
    port_byte_out(0x3D5, (port_byte_in(0x3D5) & 0xE0) | 15);
    
    cursor_x = 0;
    cursor_y = 0;
    update_cursor();
}

// Set cursor position
void set_cursor_position(int x, int y) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        cursor_x = x;
        cursor_y = y;
        update_cursor();
    }
}

// Move cursor relative to current position
void move_cursor(int dx, int dy) {
    int new_x = cursor_x + dx;
    int new_y = cursor_y + dy;
    
    if (new_x >= 0 && new_x < SCREEN_WIDTH && new_y >= 0 && new_y < SCREEN_HEIGHT) {
        cursor_x = new_x;
        cursor_y = new_y;
        update_cursor();
    }
}

// Get current cursor position
void get_cursor_position(int* x, int* y) {
    *x = cursor_x;
    *y = cursor_y;
}

// Update the hardware cursor
void update_cursor() {
    unsigned short position = cursor_y * SCREEN_WIDTH + cursor_x;
    
    // Tell the VGA board we are setting the high cursor byte
    port_byte_out(0x3D4, 0x0E);
    port_byte_out(0x3D5, (unsigned char)((position >> 8) & 0xFF));
    
    // Tell the VGA board we are setting the low cursor byte
    port_byte_out(0x3D4, 0x0F);
    port_byte_out(0x3D5, (unsigned char)(position & 0xFF));
}

// Scroll the screen up one line
void scroll_screen() {
    // Move everything up one line
    for (int y = 0; y < SCREEN_HEIGHT - 1; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int src_offset = ((y + 1) * SCREEN_WIDTH + x) * 2;
            int dest_offset = (y * SCREEN_WIDTH + x) * 2;
            
            video_memory[dest_offset] = video_memory[src_offset];
            video_memory[dest_offset + 1] = video_memory[src_offset + 1];
        }
    }
    
    // Clear the last line
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        putchar(' ', x, SCREEN_HEIGHT - 1, COLOR_LIGHT_GRAY);
    }
}

// Handle special keys
void handle_special_key(unsigned char scancode) {
    switch (scancode) {
        case 0x48: // Up arrow
            if (cursor_y > 0) {
                move_cursor(0, -1);
            }
            break;
        case 0x50: // Down arrow
            if (cursor_y < SCREEN_HEIGHT - 1) {
                move_cursor(0, 1);
            }
            break;
        case 0x4B: // Left arrow
            if (cursor_x > 0) {
                move_cursor(-1, 0);
            }
            break;
        case 0x4D: // Right arrow
            if (cursor_x < SCREEN_WIDTH - 1) {
                move_cursor(1, 0);
            }
            break;
    }
}