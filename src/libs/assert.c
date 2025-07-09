#include "driver/vga_driver.h"

void assert(int ok, const char* file, const unsigned int linenumber, const char* s) {

    if (!ok) {

        write_string("Assert failed, Execution trapped \n   ( ");
        write_string(file);
        write_string(" @ ");
        write_int(linenumber);
        write_string(" ) \"");
        write_string(s);
        write_string("\"");
        while (1);

    }

}

void warn(int ok, const char* file, const unsigned int linenumber, const char* s) {

    if (!ok) {

        write_string("Warn: \n");
        write_string(file);
        write_string(" @ ");
        write_int(linenumber);
        write_string(" ) \"");
        write_string(s);
        write_string("\"");

    }

}