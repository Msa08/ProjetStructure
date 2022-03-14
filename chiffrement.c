#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "prime_number.h"
#include "chiffrement.h"

long extended_gcd(long s, long t, long *u, long *v){
    if(s==0){
        *u=0;
        *v=1;
        return t;
    }
    printf("3\n");
    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u=vPrim-(t/s)*uPrim;
    *v=uPrim;
    printf("4\n");
    return gcd;
}

void generate_key_values(long p, long q, long* n, long *s, long *u){
    *n = p*q;
    long t = (p-1)*(q-1);
    long v = 1;
    while(extended_gcd(*s,t,u,&v)!=1){
        *s = rand_long(0,t-1);
    }
}

long* encrypt(char* chaine, long s, long n){
    long *crypted=(long*)malloc(chaine[strlen(chaine)]*sizeof(long));
    long c;
    int i=0;
    while(chaine[i]!='\0'){
        c=modpow(chaine[i], s, n);
        *crypted=c;
        i++;
        crypted++;
    }
    return crypted;
}