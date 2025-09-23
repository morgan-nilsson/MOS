// Copyright 2025 Morgan Nilsson

#include "libs/string.h"
#include "libs/stdtypes.h"

unsigned int strlen(const char *s) {
    if (s == 0) return 0;
    unsigned int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

void strcpy(char* dest, const char* src) {
    if (dest == NULL) return;
    if (src == NULL) return;
    int length = strlen(src);
    for (int i = 0; i < length; i++) {
        dest[i] = src[i];
    }
    dest[length] = '\0';
}

void strncpy(char* dest, const char* src, unsigned int n) {
    if (dest == NULL) return;
    if (src == NULL) return;
    usize length = strlen(src);
    if (n < length) length = n;
    for (unsigned int i = 0; i < n; i++) {
        dest[i] = src[i];
    }
    dest[length] = '\0';
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (*s1 != *s2) return *s1 < *s2 ? -1 : 1;
        s1++;
        s2++;
    }
    if (*s1 == *s2) return 0;
    return *s1 < *s2 ? -1 : 1;
}

int strncmp(const char *s1, const char *s2, unsigned int n) {
    while (*s1 && *s2 && n) {
        if (*s1 != *s2) return *s1 < *s2 ? -1 : 1;
        s1++;
        s2++;
        n--;
    }
    if (n == 0) return 0;
    if (*s1 == *s2) return 0;
    return *s1 < *s2 ? -1 : 1;
}

void strcat(char *dest, const char *src) {
    usize dest_len = strlen(dest);
    usize src_len = strlen(src);
    for (usize i = 0; i < src_len; i++) {
        dest[dest_len + i] = src[i];
    }
    dest[dest_len + src_len] = '\0';
}

void strncat(char *dest, const char *src, usize n) {
    usize dest_len = strlen(dest);
    usize src_len = strlen(src);
    if (n < src_len) src_len = n;
    for (unsigned int i = 0; i < src_len; i++) {
        dest[dest_len + i] = src[i];
    }
    dest[dest_len + src_len] = '\0';
}

char *strchr(const char *s, int c) {
    while (*s) {
        if (*s == c) return (char*)s;
        s++;
    }
    return NULL;
}

char *strrchr(const char *s, int c) {
    const char *last = NULL;
    while (*s) {
        if (*s == c) last = s;
        s++;
    }
    return (char*)last;
}

char *strstr(const char *haystack, const char *needle) {
    while (*haystack) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && *h == *n) {
            h++;
            n++;
        }
        if (*n == '\0') return (char*)haystack;
        haystack++;
    }
    return NULL;
}

char *strtok(char *str, const char *delim) {
    static char *last = NULL;
    if (str) last = str;
    if (!last) return NULL;
    char *start = last;
    while (*last) {
        const char *d = delim;
        while (*d) {
            if (*last == *d) {
                *last = '\0';
                last++;
                if (*last == '\0') last = NULL;
                return start;
            }
            d++;
        }
        last++;
    }
    last = NULL;
    return start;
}

// copys n bytes from src to dest
void memcpy(void *dest, const void *src, usize nbytes) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    for (usize i = 0; i < nbytes; i++) {
        d[i] = s[i];
    }
}

void memset(void *dest, int c, usize nbytes) {
    unsigned char *ptr = (unsigned char*)dest;
    for (usize index = 0; index < nbytes; ++index) {
        ptr[index] = c;
    }
}

int memcmp(const void *s1, const void *s2, usize n) {
    const unsigned char *a = (const unsigned char*)s1;
    const unsigned char *b = (const unsigned char*)s2;
    for (usize i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}