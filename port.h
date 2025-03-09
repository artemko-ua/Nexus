#ifndef PORT_H
#define PORT_H

// Write a byte to a port
void port_byte_out(unsigned short port, unsigned char value);

// Read a byte from a port
unsigned char port_byte_in(unsigned short port);

// Write a word to a port
void port_word_out(unsigned short port, unsigned short value);

// Read a word from a port
unsigned short port_word_in(unsigned short port);

#endif // PORT_H