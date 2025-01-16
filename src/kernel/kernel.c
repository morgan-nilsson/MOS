#include "../libs/inc/io.h"

void _start(void) { 
    putc('c', 0, 0);
    puts("Hello\0");
    while(1);
}