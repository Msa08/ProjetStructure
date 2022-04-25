#ifndef CHIFFREMENT_H
#define CHIFFREMENT_H
#include <openssl/sha.h>
#include "struct.h"
#include "structure.h"



int is_prime_naive(long p);
long modpow_naive(long a, long m, long n);
int modpow(long a, long m, long n);
int witness(long a, long b, long d, long p);
long rand_long(long low, long up);
int is_prime_miller(long p, int k);
long random_prime_number(int low_size, int up_size, int k);
long extended_gcd(long s, long t, long *u, long *v);
void generate_keys_values(long p, long q, long* n, long *s, long *u);
long* encrypt(char *chaine, long s, long n);
char* decrypt(long *crypted, int size, long  u, long n);
void print_long_vector(long *result, int size);
Signature* sign(char *mess, Key *sKey);
int hash_function(Key *key, int size);
unsigned char *str_to_hash(const char *str);


#endif