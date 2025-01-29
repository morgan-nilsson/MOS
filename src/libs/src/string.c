unsigned int strlen(const char *s) {
    if (s == 0) return 0;
    unsigned int i = 0;
    while (s[i] != '\0') i++;
    return i;
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