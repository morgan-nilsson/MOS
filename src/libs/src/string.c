unsigned int strlen(const char *s) {
    if (s == 0) return 0;
    unsigned int i = 0;
    while (s[i] != '\0') i++;
    return i;
}