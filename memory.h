
#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h> // For size_t

// Memory allocation functions
void* malloc(size_t size);
void free(void* ptr);
void* memset(void* ptr, int value, size_t num);
void* memcpy(void* destination, const void* source, size_t num);

// Memory information
void memory_init();
size_t get_total_memory();
size_t get_free_memory();

#endif // MEMORY_H

// file: src/memory.c
#include "../include/memory.h"

// Simple memory manager with a fixed heap size
#define HEAP_SIZE 1048576 // 1 MB heap
#define BLOCK_SIZE 16     // Each block is 16 bytes
#define BLOCKS (HEAP_SIZE / BLOCK_SIZE)

static unsigned char heap[HEAP_SIZE] = {0};
static unsigned char block_map[BLOCKS] = {0}; // 0 = free, 1 = used

void memory_init() {
    // Initialize all blocks as free
    memset(block_map, 0, BLOCKS);
}

void* malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Calculate needed blocks
    size_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    size_t start_block = 0;
    size_t consecutive_blocks = 0;
    
    // Find a sequence of free blocks
    for (size_t i = 0; i < BLOCKS; i++) {
        if (block_map[i] == 0) {
            if (consecutive_blocks == 0) {
                start_block = i;
            }
            consecutive_blocks++;
            
            if (consecutive_blocks >= blocks_needed) {
                // Found enough blocks, mark them as used
                for (size_t j = start_block; j < start_block + blocks_needed; j++) {
                    block_map[j] = 1;
                }
                
                return &heap[start_block * BLOCK_SIZE];
            }
        } else {
            consecutive_blocks = 0;
        }
    }
    
    // Not enough memory
    return NULL;
}

void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    
    // Check if the pointer is within our heap
    if (ptr < (void*)heap || ptr >= (void*)(heap + HEAP_SIZE)) {
        return;
    }
    
    // Calculate the block
    size_t block = ((unsigned char*)ptr - heap) / BLOCK_SIZE;
    
    // Mark consecutive used blocks as free
    while (block < BLOCKS && block_map[block] == 1) {
        block_map[block] = 0;
        block++;
    }
}

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    
    for (size_t i = 0; i < num; i++) {
        p[i] = (unsigned char)value;
    }
    
    return ptr;
}

void* memcpy(void* destination, const void* source, size_t num) {
    unsigned char* dest = (unsigned char*)destination;
    const unsigned char* src = (const unsigned char*)source;
    
    for (size_t i = 0; i < num; i++) {
        dest[i] = src[i];
    }
    
    return destination;
}

size_t get_total_memory() {
    return HEAP_SIZE;
}

size_t get_free_memory() {
    size_t free_blocks = 0;
    
    for (size_t i = 0; i < BLOCKS; i++) {
        if (block_map[i] == 0) {
            free_blocks++;
        }
    }
    
    return free_blocks * BLOCK_SIZE;
}