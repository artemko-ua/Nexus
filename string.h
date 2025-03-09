#ifndef STRING_H
#define STRING_H

#include <stddef.h> // For size_t

// String functions
size_t strlen(const char* str);
char* strcpy(char* destination, const char* source);
char* strncpy(char* destination, const char* source, size_t num);
char* strcat(char* destination, const char* source);
char* strncat(char* destination, const char* source, size_t num);
int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t num);
char* strchr(const char* str, int character);
char* strrchr(const char* str, int character);
char* strtok(char* str, const char* delimiters);

#endif // STRING_H

// file: src/string.c
#include "../include/string.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char* strcpy(char* destination, const char* source) {
    size_t i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
    return destination;
}

char* strncpy(char* destination, const char* source, size_t num) {
    size_t i;
    
    for (i = 0; i < num && source[i] != '\0'; i++) {
        destination[i] = source[i];
    }
    
    for (; i < num; i++) {
        destination[i] = '\0';
    }
    
    return destination;
}

char* strcat(char* destination, const char* source) {
    size_t dest_len = strlen(destination);
    size_t i = 0;
    
    while (source[i] != '\0') {
        destination[dest_len + i] = source[i];
        i++;
    }
    
    destination[dest_len + i] = '\0';
    return destination;
}

char* strncat(char* destination, const char* source, size_t num) {
    size_t dest_len = strlen(destination);
    size_t i;
    
    for (i = 0; i < num && source[i] != '\0'; i++) {
        destination[dest_len + i] = source[i];
    }
    
    destination[dest_len + i] = '\0';
    return destination;
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    
    return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

int strncmp(const char* str1, const char* str2, size_t num) {
    for (size_t i = 0; i < num; i++) {
        if (str1[i] != str2[i]) {
            return (int)(unsigned char)str1[i] - (int)(unsigned char)str2[i];
        }
        
        if (str1[i] == '\0') {
            return 0;
        }
    }
    
    return 0;
}

char* strchr(const char* str, int character) {
    while (*str != '\0') {
        if (*str == (char)character) {
            return (char*)str;
        }
        str++;
    }
    
    if ((char)character == '\0') {
        return (char*)str;
    }
    
    return NULL;
}

char* strrchr(const char* str, int character) {
    const char* last_occurrence = NULL;
    
    while (*str != '\0') {
        if (*str == (char)character) {
            last_occurrence = str;
        }
        str++;
    }
    
    if ((char)character == '\0') {
        return (char*)str;
    }
    
    return (char*)last_occurrence;
}

// Helper variables for strtok
static char* strtok_str = NULL;
static char* strtok_saved_ptr = NULL;

char* strtok(char* str, const char* delimiters) {
    char* token_start;
    
    // If str is NULL, use the saved pointer
    if (str == NULL) {
        str = strtok_saved_ptr;
    }
    
    // Skip leading delimiters
    while (*str != '\0' && strchr(delimiters, *str) != NULL) {
        str++;
    }
    
    // If we've reached the end, return NULL
    if (*str == '\0') {
        strtok_saved_ptr = str;
        return NULL;
    }
    
    // Mark the start of the token
    token_start = str;
    
    // Find the end of the token
    while (*str != '\0' && strchr(delimiters, *str) == NULL) {
        str++;
    }
    
    // If we found a delimiter, replace it with NULL and save the pointer
    if (*str != '\0') {
        *str = '\0';
        strtok_saved_ptr = str + 1;
    } else {
        strtok_saved_ptr = str;
    }
    
    return token_start;
}