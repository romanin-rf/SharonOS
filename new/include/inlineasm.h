#ifndef _INLINEASM_H
#define _INLINEASM_H

unsigned char inb (unsigned short _port); 
unsigned char inw (unsigned short _port);

void outb (unsigned short _port, unsigned char _data);
void outw (unsigned short _port, unsigned char _data);

#endif