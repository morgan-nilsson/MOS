#ifndef STRING_H
#define STRING_H

#ifndef NULL
#define NULL ((void *)0)
#endif
// length of a null termanating string
unsigned int strlen(const char *s);
void strcpy(char* dest, const char* src);
void strncpy(char* dest, const char* src, unsigned int n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, unsigned int n);

void memcpy(void *dest, const void* src, unsigned int nbytes);
void memset(void *dest, int c, unsigned int nbytes);

#endif