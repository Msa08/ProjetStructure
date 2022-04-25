#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include "chiffrement.h"
#include "struct.h"
#include <math.h>
#include <assert.h>

/* == 1.1 == :
retourne 1 si p est premier et -1 ou 0 sinon
*/

int is_prime_naive(long p){
    /* 
    p est un entier impair
    ses premieres valeurs sont donc {1,3,5,7...}
    */

    // cas ou p est pair (!= premier)
    if (p%2==0)
        return -1;

    // tous les cas entre 3 et p-1 :
    for(int i=3; i<p;i++){
        if (p%i==0)
            return 0;
    }

    return 1;
}

/* == 1.3 == :
calcule (a^m mod n) de manière naive 
*/

long modpow_naive(long a, long m, long n){
    int i = 0;
    long res=1;

    while (i<m){
        res=(res*a)%n;
        i++;
    }

    return res;

}

/* == 1.4 == :
calcule (a^m mod n) de manière recursive selon la valeur de m 
*/

int modpow(long a, long m, long n){
    long tmp;
    if (m==0)
        return 1;

    if (m%2 == 0){
        tmp = modpow(a, m/2, n);
        return (tmp*tmp)%n;
    }

    if (m%2 != 0){
        tmp = modpow(a, m/2, n);
        return (a*tmp*tmp)%n;
    }
    return 0;
}

/* == 1.6 == :
FONCTIONS PREDEFINIES 
*/
int witness(long a, long b, long d, long p){
    long x = modpow(a, d, p) ;
    if ( x == 1) {
        return 0;
    }
    for (long i = 0; i < b; i ++){
        if(x == p -1){
            return 0;
        }
        x = modpow(x, 2, p);
    }
    return 1;
}

long rand_long(long low, long up){
    return rand() % ( up - low +1)+low;
}

int is_prime_miller(long p, int k){
    if ( p == 2) {
        return 1;
    }
    if (!(p & 1) || p <= 1){ //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1)){ //tant que d n’est pas impair
        d = d/2;
        b = b+1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a ;
    int i ;
    for ( i = 0; i < k ; i ++){
        a = rand_long(2 , p -1);
        if (witness(a,b,d,p)){
            return 0;
        }
    }
    return 1;
}

/* == 1.8 == :
retourne un nombre premier de taille comprise entre low_size et up_size
en effectuant k tests de Miller pour tester la primalite du nombre genere. 
*/


long random_prime_number(int low_size, int up_size, int k){
    int min = pow(2,low_size-1);
    int max = pow(2, up_size)-1;
    long rand = rand_long(min, max);
    int i = is_prime_miller(rand, k);
    while (i!=1){
        rand=rand_long(min, max);
        i = is_prime_miller(rand, k);
    }

    return rand;
}

/* == EXERCICE 2 == :
Version recursive de l'algorithme d'EUCLIDE (fournie) 
*/


long extended_gcd(long s, long t, long *u, long *v){
    if (s==0){
        *u = 0;
        *v = 1;
        return t;
    }

    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u = vPrim-(t/s)*uPrim;
    *v = uPrim;

    return (long)gcd;

}

/* == 2.1 == :
genere les cles publique et secrete en appelant la fonction 
extended_gcd et en mettant a jours n, s et p
*/

void generate_keys_values(long p, long q, long* n, long *s, long *u){
    // calcul de n :
    *n = p*q;
    // calcul de t 
    long t = (p-1)*(q-1);

    // calcul de s :
    long gcd = 0;
    *s = rand_long(0, t);
    long v;
    while (gcd!=1 || *u<0){
        *s = rand_long(0, t);
        gcd = extended_gcd(*s, t, u, &v);
    }

       
}

/* == 2.2 == :
chiffre le message caractere par caractere 
*/

long* encrypt(char * chaine, long s, long n){
    // valeur de retour du message crypte 
    long* tabcrypt=(long *)malloc(sizeof(long)*strlen(chaine));
    int i = 0;

    // cryptage caractere par caractere 
    while(chaine[i] != '\0'){
        tabcrypt[i] = (long)modpow((long)((int)(chaine[i])), s, n);
        i++;
    }
    return tabcrypt;
}

/* == 2.3 == :
dechiffre le message caractere par caractere 
*/

char* decrypt(long * crypted, int size, long  u, long n){
    // chaine de retour 
    char *message=(char*)malloc(sizeof(char)*size+1);

    // cryptage caractere par caractere 
    for(int i=0; i<size; i++){
        message[i]=(char)modpow(crypted[i], u, n);
    }

    message[strlen(message)+1]='\0';
    return message;
}

void print_long_vector(long *result, int size){
    printf ("Vector:[") ;
    for (int i=0; i<size; i++){
        printf("%lx\t",result[i]) ;
    }
    printf ("]\n" ) ;
}

/* == 3.7 == :
Cree une signature a partir de la declaration de 
vote (chaine de char de la clé publique du candidat vote)
et d'une cle
*/

Signature* sign(char* mess, Key* sKey){
    // encryptage de la declaration grace a la cle secrete de l'electeur
    long *content = encrypt(mess, sKey->val, sKey->n);
    assert(content);
    // creation et init de la signature 
    Signature *sign = init_signature(content, strlen(mess));
    return sign;
}

int hash_function(Key* key, int size){

    char *cle = key_to_str(key);
    float A=(sqrt(5)-1)/2.0;

    int somme = 0;// pour additionner le nombre représentant chaque lettre de la cle
    int i = 0;//index de tableau
    while(cle[i]!='\0'){ //chaque chaîne de caractère finit par '\0'
        somme+=(int)(cle[i]);//cast en int pour avoir le nombre et non la lettre
        i++;
    }

    free(cle);
    return (int)(size*((somme*A)-((int)(somme*A))));

}

unsigned char *str_to_hash(const char * str){

    unsigned char *d = SHA256(str, strlen(str), 0);
    char *string = malloc(sizeof(char)*(SHA256_DIGEST_LENGTH*2+1));
    int cpt=0;
    for(int it = 0; it<SHA256_DIGEST_LENGTH; it++){
        sprintf(string+cpt, "%02x", d[it]);
        cpt+=2;
    }
    return string;
}
