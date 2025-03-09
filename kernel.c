// file: kernel.c
#include "screen.h"
#include "keyboard.h"
#include "filesystem.h"
#include "shell.h"

void kernel_main() {
    // Initialize screen
    clear_screen();
    init_cursor();
    
    // Initialize keyboard
    keyboard_init();
    
    // Initialize filesystem
    filesystem_t filesystem;
    fs_init(&filesystem);
    
    // Display welcome message
    print("Nexux OS Kernel", 30, 10, 0x0F); // White text on black background
    print("Version 0.1", 33, 12, 0x07);     // Gray text on black background
    print("Press any key to continue...", 25, 14, 0x07);
    
    // Wait for keypress
    while (keyboard_process_key() == 0) {
        // Just wait
    }
    
    // Initialize and run the shell
    shell_init(&filesystem);
    shell_run();
    
    // This code should never be reached because shell_run() contains an infinite loop
    // But just in case, loop forever
    while (1) {
        // Do nothing
    }
}