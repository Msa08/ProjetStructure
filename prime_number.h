#ifndef PRIME_NUMBER_H
#define PRIME_NUMBER_H

int is_prime_naive(long p);
long modpow_naive(long a, long m, long n);
long modpow(long a, long m, long n);
int is_prime_miller(long p, int k);
long rand_long(long low, long up);
int witness(long a, long b, long d, long p);
long random_prime_number(int low_size, int up_size, int k);
#endif
