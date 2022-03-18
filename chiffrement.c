#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "prime_number.h"
#include "chiffrement.h"

long extended_gcd(long s, long t, long *u, long *v){
    if(s==0){
        *u=0;
        *v=1;
        return t;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u=vPrim-((t/s)*uPrim);
    *v=uPrim;
    return gcd;
}

void generate_key_values(long p, long q, long* n, long *s, long *u){
    srand(time(NULL));
    *n = p*q;
    long t = (p-1)*(q-1);
    long v = 1;
    while(extended_gcd(*s,t,u,&v)!=1){
        *s = rand_long(0,t);
    }
    printf("%ld %ld %ld %ld %ld\n",*s,*u,t,v,((*s)*(*u))+(t*v));
}

long* encrypt(char* chaine, long s, long n){
    long *crypted=(long*)malloc(chaine[strlen(chaine)]*sizeof(long));
    int i=0;
    while(chaine[i]!='\0'){
        crypted[i]=modpow(chaine[i], s, n);
        i++;
    }
    return crypted;
}

char* decrypt(long* crypted, int size, long u, long n){
    char* decrypt=malloc(size*sizeof(char));
    for(int i=0;i<size;i++){
        decrypt[i]=modpow(crypted[i],u,n);
        printf("%lx\n",modpow(crypted[i],u,n));
    }
    return decrypt;

}