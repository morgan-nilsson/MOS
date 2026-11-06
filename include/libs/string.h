// Copyright 2025 Morgan Nilsson

#ifndef SRC_LIBS_INCLUDE_STRING_H_
#define SRC_LIBS_INCLUDE_STRING_H_

#include <stddef.h>

#ifndef NULL
#define NULL ((void *)0)
#endif
// length of a null termanating string
unsigned int strlen(const char *s);
void strcpy(char* dest, const char* src);
void strncpy(char* dest, const char* src, unsigned int n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, unsigned int n);

void memcpy(void *dest, const void* src, size_t nbytes);
void memset(void *dest, int c, size_t nbytes);

#endif