#ifndef STDIO_H
#define STDIO_H

#ifdef NULL
#define NULL ((void *)0)
#endif

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

#endif