// Copyright 2025 Morgan Nilsson

#ifndef SRC_LIBS_INCLUDE_STDIO_H_
#define SRC_LIBS_INCLUDE_STDIO_H_

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

#endif