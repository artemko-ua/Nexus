#include "shell.h"
#include "screen.h"
#include "keyboard.h"
#include <string.h> // For strcmp, strcpy, strlen

// Global filesystem
static filesystem_t* filesystem;

// Available commands
static command_t commands[] = {
    {"help", "Display this help message", cmd_help},
    {"clear", "Clear the screen", cmd_clear},
    {"echo", "Print text to the screen", cmd_echo},
    {"ls", "List files", cmd_ls},
    {"cat", "View file contents", cmd_cat},
    {"touch", "Create a new file", cmd_touch},
    {"rm", "Remove a file", cmd_rm},
    {"write", "Write text to a file", cmd_write},
    {"pwd", "Print working directory", cmd_pwd},
    {"", "", NULL} // End marker
};

void shell_init(filesystem_t* fs) {
    filesystem = fs;
}

void shell_run() {
    char command[MAX_COMMAND_LENGTH];
    char* args[MAX_ARGS];
    int num_args;
    
    clear_screen();
    print("Nexux OS - A Minimal Unix-Like Operating System\n", 0, 0, COLOR_WHITE);
    print("Type 'help' for a list of commands\n\n", 0, 1, COLOR_LIGHT_GRAY);
    
    while (1) {
        // Print the prompt
        print_at_cursor(filesystem->current_directory);
        print_at_cursor("$ ");
        
        // Get command
        keyboard_get_line(command, MAX_COMMAND_LENGTH);
        print_at_cursor("\n");
        
        // Parse command into arguments
        num_args = 0;
        char* token = strtok(command, " ");
        while (token != NULL && num_args < MAX_ARGS) {
            args[num_args] = token;
            num_args++;
            token = strtok(NULL, " ");
        }
        
        // Make sure the command isn't empty
        if (num_args == 0) {
            continue;
        }
        
        // Find and execute the command
        int command_found = 0;
        for (int i = 0; commands[i].function != NULL; i++) {
            if (strcmp(args[0], commands[i].name) == 0) {
                commands[i].function(num_args, args);
                command_found = 1;
                break;
            }
        }
        
        if (!command_found) {
            print_at_cursor("Command not found: ");
            print_at_cursor(args[0]);
            print_at_cursor("\n");
        }
    }
}

// Help command
int cmd_help(int argc, char* argv[]) {
    print_at_cursor("Available commands:\n");
    
    for (int i = 0; commands[i].function != NULL; i++) {
        print_at_cursor("  ");
        print_at_cursor(commands[i].name);
        
        // Pad with spaces for alignment
        int padding = 10 - strlen(commands[i].name);
        for (int j = 0; j < padding; j++) {
            print_at_cursor(" ");
        }
        
        print_at_cursor(commands[i].description);
        print_at_cursor("\n");
    }
    
    return 0;
}

// Clear command
int cmd_clear(int argc, char* argv[]) {
    clear_screen();
    return 0;
}

// Echo command
int cmd_echo(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        print_at_cursor(argv[i]);
        if (i < argc - 1) {
            print_at_cursor(" ");
        }
    }
    print_at_cursor("\n");
    return 0;
}

// List files command
int cmd_ls(int argc, char* argv[]) {
    for (unsigned int i = 0; i < filesystem->count; i++) {
        print_at_cursor(filesystem->files[i].filename);
        print_at_cursor("  ");
    }
    print_at_cursor("\n");
    return 0;
}

// Cat command
int cmd_cat(int argc, char* argv[]) {
    if (argc < 2) {
        print_at_cursor("Usage: cat <filename>\n");
        return 1;
    }
    
    char buffer[MAX_FILE_SIZE + 1];
    int size = fs_read_file(filesystem, argv[1], buffer, MAX_FILE_SIZE);
    
    if (size < 0) {
        print_at_cursor("File not found: ");
        print_at_cursor(argv[1]);
        print_at_cursor("\n");
        return 1;
    }
    
    buffer[size] = '\0';
    print_at_cursor(buffer);
    print_at_cursor("\n");
    
    return 0;
}

// Touch command
int cmd_touch(int argc, char* argv[]) {
    if (argc < 2) {
        print_at_cursor("Usage: touch <filename>\n");
        return 1;
    }
    
    int result = fs_create_file(filesystem, argv[1]);
    
    if (result < 0) {
        if (result == -1) {
            print_at_cursor("Filesystem full\n");
        } else if (result == -2) {
            print_at_cursor("File already exists: ");
            print_at_cursor(argv[1]);
            print_at_cursor("\n");
        }
        return 1;
    }
    
    return 0;
}

// Remove command
int cmd_rm(int argc, char* argv[]) {
    if (argc < 2) {
        print_at_cursor("Usage: rm <filename>\n");
        return 1;
    }
    
    int result = fs_delete_file(filesystem, argv[1]);
    
    if (result < 0) {
        print_at_cursor("File not found: ");
        print_at_cursor(argv[1]);
        print_at_cursor("\n");
        return 1;
    }
    
    return 0;
}

// Write command
int cmd_write(int argc, char* argv[]) {
    if (argc < 3) {
        print_at_cursor("Usage: write <filename> <text>\n");
        return 1;
    }
    
    // Check if file exists, create if not
    if (fs_exists(filesystem, argv[1]) < 0) {
        fs_create_file(filesystem, argv[1]);
    }
    
    // Combine all remaining arguments into a single text
    char text[MAX_FILE_SIZE] = "";
    for (int i = 2; i < argc; i++) {
        strcat(text, argv[i]);
        if (i < argc - 1) {
            strcat(text, " ");
        }
    }
    
    int result = fs_write_file(filesystem, argv[1], text, strlen(text));
    
    if (result < 0) {
        print_at_cursor("Failed to write to file: ");
        print_at_cursor(argv[1]);
        print_at_cursor("\n");
        return 1;
    }
    
    return 0;
}

// Print working directory command
int cmd_pwd(int argc, char* argv[]) {
    print_at_cursor(filesystem->current_directory);
    print_at_cursor("\n");
    return 0;