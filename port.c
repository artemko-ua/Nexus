#include "port.h"

// Write a byte to a port
void port_byte_out(unsigned short port, unsigned char value) {
    __asm__("outb %0, %1" : : "a"(value), "Nd"(port));
}

// Read a byte from a port
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Write a word to a port
void port_word_out(unsigned short port, unsigned short value) {
    __asm__("outw %0, %1" : : "a"(value), "Nd"(port));
}

// Read a word from a port
unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}
