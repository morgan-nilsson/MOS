// Copyright 2025 Morgan Nilsson

#include "../include/ctype.h"

int isalpah(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isdigit(int c) {
    return c >= '0' && c <= '9';
}

int isalnum(int c) {
    return isalpah(c) || isdigit(c);
}

int islower(int c) {
    return c >= 'a' && c <= 'z';
}

int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}

int tolower(int c) {
    if (isupper(c)) {
        return c + 32;
    }
    return c;
}

int toupper(int c) {
    if (islower(c)) {
        return c - 32;
    }
    return c;
}

int isspace(int c) {
    return c == ' ' || c == '\t' || c == '\n';
}