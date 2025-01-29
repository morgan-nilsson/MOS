#include "../inc/string.h"

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
    int length = strlen(src);
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

// copys n bytes from src to dest
void memcpy(void *dest, const void *src, unsigned int nbytes) {
    unsigned char *d = (unsigned char*)dest;
    const unsigned char *s = (const unsigned char*)src;

    for (unsigned int i = 0; i < nbytes; i++) {
        d[i] = s[i];
    }
}

void memset(void *dest, int c, unsigned int nbytes) {
    unsigned char *ptr = (unsigned char*)dest;
    for (int index = 0; index < nbytes; ++index) {
        ptr[index] = c;
    }
}

int memcmp(const void *s1, const void *s2, unsigned int n) {
    const unsigned char *a = (const unsigned char*)s1;
    const unsigned char *b = (const unsigned char*)s2;
    for (unsigned int i = 0; i < n; i++) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}