#include <stdio.h>
#include <stdlib.h>
#include "key.h"
#include "prime_number.h"
#include "chiffrement.h"
#include "math.h"
void init_key(Key* key, long val, long n){
    key->n=n;
    key->clé=val;
}

void init_pair_key(Key* pKey, Key* sKey, long low_size, long up_size){
    long a=random_prime_number(low_size,up_size,5000);
    long b=random_prime_number(low_size,up_size,5000);
    while(a==b){
        b=random_prime_number(low_size,up_size,5000);
    }
    long n;
    long u;
    long s;
    generate_key_values(a,b, &n, &s, &u);
    if(u<0){
        long t= (a-1)*(b-1);
        u=u+t;
    }
    pKey->clé=s;
    pKey->n=n;
    sKey->clé=u;
    sKey->n=n;
}

char* key_to_str(Key* key){
    char* str=malloc(10*sizeof(char));
    int i=0;
    long a=key->clé;
    long b=key->n;
    str[0]='(';
    str[1]=0xa;
    str[2]=',';
    str[3]=0xb;
    str[4]=')';
    return str;
} 