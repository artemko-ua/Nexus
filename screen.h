#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN 0x6
#define COLOR_LIGHT_GRAY 0x7
#define COLOR_DARK_GRAY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xA
#define COLOR_LIGHT_CYAN 0xB
#define COLOR_LIGHT_RED 0xC
#define COLOR_LIGHT_MAGENTA 0xD
#define COLOR_YELLOW 0xE
#define COLOR_WHITE 0xF

// Basic screen functions
void clear_screen();
void putchar(char c, int x, int y, char attribute);
void print(const char* str, int x, int y, char attribute);
void print_at_cursor(const char* str);
void backspace();
void update_cursor();
void scroll_screen();

// Cursor functions
void init_cursor();
void set_cursor_position(int x, int y);
void move_cursor(int dx, int dy);
void get_cursor_position(int* x, int* y);

// Special keys
void handle_special_key(unsigned char scancode);

// Variables for the cursor position
extern int cursor_x;
extern int cursor_y;

#endif // SCREEN_H