#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25


// write a char to thy screen
void putc(const unsigned char c);
void puts(const unsigned char *s);