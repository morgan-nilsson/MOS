#ifndef STRING_H
#define STRING_H

// length of a null termanating string
unsigned int strlen(const char *s);
void memcpy(void *dest, void* src, unsigned int length);
void memset(void *dest, int c, unsigned int nbytes);

#endif