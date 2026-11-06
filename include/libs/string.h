// Copyright 2025 Morgan Nilsson

#ifndef SRC_LIBS_INCLUDE_STRING_H_
#define SRC_LIBS_INCLUDE_STRING_H_

#include <stddef.h>

#ifndef NULL
#define NULL ((void *)0)
#endif
// length of a null termanating string
size_t strlen(const char *s);
void strcpy(char* dest, const char* src);
void strncpy(char* dest, const char* src, size_t n);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

void strcat(char *dest, const char *src);
void strncat(char *dest, const char *src, size_t n);
char *strchr(const char *s, int c);
char *strrchr(const char *s, int c);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);

void memcpy(void *dest, const void* src, size_t nbytes);
void memset(void *dest, int c, size_t nbytes);
int memcmp(const void *s1, const void *s2, size_t nbytes);

#endif