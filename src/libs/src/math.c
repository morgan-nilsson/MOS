// Copyright 2025 Morgan Nilsson

#include "../include/math.h"

int pow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

long int lpow(long int base, long int exp) {
    long int result = 1;
    for (long int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

long long int llpow(long long int base, long long int exp) {
    long long int result = 1;
    for (long long int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int sqrt(int n) {
    int result = 0;
    for (int i = 0; i <= n; i++) {
        if (i * i == n) {
            result = i;
            break;
        }
    }
    return result;
}

long int lsqrt(long int n) {
    long int result = 0;
    for (long int i = 0; i <= n; i++) {
        if (i * i == n) {
            result = i;
            break;
        }
    }
    return result;
}

long long int llsqrt(long long int n) {
    long long int result = 0;
    for (long long int i = 0; i <= n; i++) {
        if (i * i == n) {
            result = i;
            break;
        }
    }
    return result;
}

int factorial(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

long int lfactorial(long int n) {
    long int result = 1;
    for (long int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

long long int llfactorial(long long int n) {
    long long int result = 1;
    for (long long int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

double ceil(double x) {
    int int_x = (int)x;
    return int_x == x ? x : int_x + 1;
}

double floor(double x) {
    return (int)x;
}

double round(double x) {
    return x - (int)x >= 0.5 ? ceil(x) : floor(x);
}

double fmod(double x, double y) {
    return x - y * (int)(x / y);
}

double sin(double x) {
    // Normalize x to the range [-pi, pi]
    x = fmod(x, 2 * PI);
    if (x > PI) x -= 2 * PI;
    if (x < -PI) x += 2 * PI;

    double result = x;
    double term = x;
    int i;
    for (i = 3; i < 10; i += 2) {
        term *= (-1) * x * x / (i * (i - 1));
        result += term;
    }

    return result;
}

double cos(double x) {
    // Normalize x to the range [-pi, pi]
    x = fmod(x, 2 * PI);
    if (x > PI) x -= 2 * PI;
    if (x < -PI) x += 2 * PI;

    double result = 1;
    double term = 1;
    int i;
    for (i = 2; i < 10; i += 2) {
        term *= (-1) * x * x / (i * (i - 1));
        result += term;
    }

    return result;
}

double tan(double x) {
    return sin(x) / cos(x);
}

double asin(double x) {
    double result = x;
    double term = x;
    int i;
    for (i = 3; i < 10; i += 2) {
        term *= x * x * (i - 2) / (i * (i - 1));
        result += term;
    }

    return result;
}

double acos(double x) {
    return PI / 2 - asin(x);
}

double atan(double x) {
    double result = x;
    double term = x;
    int i;
    for (i = 3; i < 10; i += 2) {
        term *= x * x * (i - 2) / (i * (i - 1));
        result += term;
    }

    return result;
}

double sinh(double x) {
    double result = x;
    double term = x;
    int i;
    for (i = 3; i < 10; i += 2) {
        term *= x * x / (i * (i - 1));
        result += term;
    }

    return result;
}

double cosh(double x) {
    double result = 1;
    double term = 1;
    int i;
    for (i = 2; i < 10; i += 2) {
        term *= x * x / (i * (i - 1));
        result += term;
    }

    return result;
}

double tanh(double x) {
    return sinh(x) / cosh(x);
}

double exp(double x) {
    double result = 1;
    double term = 1;
    int i;
    for (i = 1; i < 10; i++) {
        term *= x / i;
        result += term;
    }

    return result;
}

double log(double x) {
    double result = 0;
    double term = x - 1;
    int i;
    for (i = 2; i < 10; i++) {
        term *= (x - 1) * (i - 1) / i;
        result += term;
    }

    return result;
}