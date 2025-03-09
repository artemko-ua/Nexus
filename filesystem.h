#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_FILES 64
#define MAX_FILENAME 32
#define MAX_FILE_SIZE 4096

typedef struct {
    char filename[MAX_FILENAME];
    char data[MAX_FILE_SIZE];
    unsigned int size;
    unsigned char attributes; // 0x01 = read-only, 0x02 = hidden, etc.
} file_t;

typedef struct {
    file_t files[MAX_FILES];
    unsigned int count;
    char current_directory[MAX_FILENAME];
} filesystem_t;

// Initialize filesystem
void fs_init(filesystem_t* fs);

// File operations
int fs_create_file(filesystem_t* fs, const char* filename);
int fs_delete_file(filesystem_t* fs, const char* filename);
int fs_read_file(filesystem_t* fs, const char* filename, char* buffer, unsigned int max_size);
int fs_write_file(filesystem_t* fs, const char* filename, const char* data, unsigned int size);
int fs_exists(filesystem_t* fs, const char* filename);
void fs_list_files(filesystem_t* fs);

#endif // FILESYSTEM_H

// file: src/filesystem.c
#include "filesystem.h"
#include <stddef.h> // For NULL
#include <string.h> // For strcmp, strcpy, strlen

void fs_init(filesystem_t* fs) {
    fs->count = 0;
    strcpy(fs->current_directory, "/");
    
    // Create root directory
    fs_create_file(fs, "README.txt");
    fs_write_file(fs, "README.txt", "Welcome to Nexux OS!\nType 'help' for a list of commands.", 52);
}

int fs_create_file(filesystem_t* fs, const char* filename) {
    if (fs->count >= MAX_FILES) {
        return -1; // Filesystem full
    }
    
    if (fs_exists(fs, filename) >= 0) {
        return -2; // File already exists
    }
    
    strcpy(fs->files[fs->count].filename, filename);
    fs->files[fs->count].size = 0;
    fs->files[fs->count].attributes = 0;
    fs->count++;
    
    return 0; // Success
}

int fs_delete_file(filesystem_t* fs, const char* filename) {
    int index = fs_exists(fs, filename);
    
    if (index < 0) {
        return -1; // File not found
    }
    
    // Move the last file to the position of the deleted file
    if (index < fs->count - 1) {
        fs->files[index] = fs->files[fs->count - 1];
    }
    
    fs->count--;
    return 0; // Success
}

int fs_read_file(filesystem_t* fs, const char* filename, char* buffer, unsigned int max_size) {
    int index = fs_exists(fs, filename);
    
    if (index < 0) {
        return -1; // File not found
    }
    
    unsigned int size_to_copy = fs->files[index].size;
    if (size_to_copy > max_size) {
        size_to_copy = max_size;
    }
    
    memcpy(buffer, fs->files[index].data, size_to_copy);
    return size_to_copy;
}

int fs_write_file(filesystem_t* fs, const char* filename, const char* data, unsigned int size) {
    int index = fs_exists(fs, filename);
    
    if (index < 0) {
        return -1; // File not found
    }
    
    if (size > MAX_FILE_SIZE) {
        size = MAX_FILE_SIZE;
    }
    
    memcpy(fs->files[index].data, data, size);
    fs->files[index].size = size;
    
    return size;
}

int fs_exists(filesystem_t* fs, const char* filename) {
    for (unsigned int i = 0; i < fs->count; i++) {
        if (strcmp(fs->files[i].filename, filename) == 0) {
            return i;
        }
    }
    
    return -1; // File not found
}

void fs_list_files(filesystem_t* fs) {
    for (unsigned int i = 0; i < fs->count; i++) {
        // Code to print filenames will be in shell.c
    }
}