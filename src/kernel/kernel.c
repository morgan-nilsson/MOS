#include "../libs/inc/io.h"
#include "../libs/inc/vga_driver.h"
#include "../libs/inc/string.h"

void _start(void) { 
    const char* msg = "Hello world\n";

    write_string(msg);

    while(1);
}