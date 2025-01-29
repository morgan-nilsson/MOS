#include "../libs/inc/vga_driver.h"
#include "../libs/inc/string.h"

void _start(void) { 
    char *str = "a\n";

    for (int i = 'a'; i <= 'z'; i++) {
        str[0] = i;
        write_string(str);
    }

    while(1);
}