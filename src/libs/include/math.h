// Copyright 2025 Morgan Nilsson

#ifndef SRC_LIBS_INCLUDE_MATH_H_
#define SRC_LIBS_INCLUDE_MATH_H_

#define PI 3.14159265358979323846
#define E 2.71828182845904523536

extern int abs(int n);
long int labs(long int n);
long long int llabs(long long int n);

int max(int a, int b);
int min(int a, int b);
long int lmax(long int a, long int b);
long int lmin(long int a, long int b);
long long int llmax(long long int a, long long int b);
long long int llmin(long long int a, long long int b);

int pow(int base, int exp);
long int lpow(long int base, long int exp);
long long int llpow(long long int base, long long int exp);

int sqrt(int n);
long int lsqrt(long int n);
long long int llsqrt(long long int n);

int factorial(int n);
long int lfactorial(long int n);
long long int llfactorial(long long int n);

double ceil(double x);
double floor(double x);
double round(double x);

double fmod(double x, double y);

double sin(double x);
double cos(double x);
double tan(double x);

double asin(double x);
double acos(double x);
double atan(double x);

double sinh(double x);
double cosh(double x);
double tanh(double x);

double exp(double x);
double log(double x);


#endif