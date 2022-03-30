#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "prime_number.h"
#include "chiffrement.h"

long extended_gcd(long s, long t, long *u, long *v){
    /*Retourne le PGCD(s,t) et affecte à u et v les valeurs
    verifiant l'equation de Bezout : s*u +t*v =PGCD(s,t)   */
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
    /*Génère la clé public pkey(s,n) et la clé secrete skey(u,n) à partir des
    nombres premiers p et q */
    *n = p*q;
    long t = (p-1)*(q-1);
    long v = 1;
    //Tant que PGCD(s,t) ne vaut pas 1 itérer
    while(extended_gcd(*s,t,u,&v)!=1){
        *s = rand_long(0,t);// s= long généré aleatoirement compris en 0 et t
    }
}

long* encrypt(char* chaine, long s, long n){
    /*Retourne la chaine chiffré grace à la clé publique */
    long *crypted=(long*)malloc(strlen(chaine)*sizeof(long));
    int i=0;
    while(chaine[i]!='\0'){
        crypted[i]=modpow(chaine[i], s, n);// à chaque lettre de chaine on applique l'exponentiation modulaire
        i++;
    }
    return crypted;
}

char* decrypt(long* crypted, int size, long u, long n){
    /*Retourne crypted sous forme de message déchiffré*/
    char* decrypt=malloc(size*sizeof(char)+1);
    for(int i=0;i<size;i++){
        decrypt[i]=(char)modpow(crypted[i],u,n);//on applique l'exponentiation modulaire
    }
    decrypt[size]='\0';
    return decrypt;

}