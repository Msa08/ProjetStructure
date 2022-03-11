#ifndef CHIFFREMENT_H
#define CHIFFREMENT_H

long extended_gcd(long s, long t, long *u, long *v);
void generate_key_values(long p, long q, long* n, long *s, long *u);
#endif