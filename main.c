#include <stdio.h>
#include <stdlib.h>
#include "prime_number.h"
#include "chiffrement.h"
#include "key.h"
#include <time.h>
#include <string.h>

void print_long_vector(long *result, int size){
    printf("Vector: [");
    for(int i=0;i<size;i++){
        printf("%lx \t",result[i]);
    }
    printf("]\n");
}
int main(){
    //FILE *f = fopen("modpow.txt", "w");
    //FILE *f2=fopen("modpow2.txt", "w");
    /*clock_t time_initial=0;
    clock_t time_final=0;
    clock_t time_initial2=0;
    clock_t time_final2=0;
    double time;
    double time2;
    long a, b;
    for(int i=20;i<40;i++){
        a=modpow(10,i,15);
        b=modpow_naive(10,i,15);
        printf("modpow : %lu, naive : %lu\n",a,b);
    }
    long i;
    int res=0;
    int tmp=0;
    for(i=215000000; time<2;i++){
        time_initial=clock();
        tmp=is_prime_naive(i);
        time_final=clock();
        if(tmp==1){
            res=i;
        }
        time=(double)(time_final-time_initial)/CLOCKS_PER_SEC;
        printf("time : %f, nb : %d\n",time, i);
    }
    printf("plus grand nombre : %d\n",res);
    return 0 ;

    for(int i=1;i<1000000;i++){
      time_initial=clock();
      modpow(10, i, 15);
      time_final=clock();
      time=(double)(time_final-time_initial)/CLOCKS_PER_SEC;
      time_initial2=clock();
      modpow_naive(10, i, 15);
      time_final2=clock();
      time2=(double)(time_final2-time_initial2)/CLOCKS_PER_SEC;
      fprintf(f,"%d %f\n",i,time);
      fprintf(f2,"%d %f\n",i,time2);
    }
    fclose(f);
    fclose(f2);*/

    srand(time(NULL));

    //generation de cle;
    /*long a=random_prime_number(3,7,5000);
    long b=random_prime_number(3,7,5000);
    while(a==b){
        b=random_prime_number(3,7,5000);
    }
    printf("nb premier %lu %lu\n",a,b);

    long n;
    long u;
    long s;
    generate_key_cleues(a,b, &n, &s, &u);
    if(u<0){
        long t= (a-1)*(b-1);
        u=u+t;
    }

    //affichage des cles en haxadécimal
    printf("cle publique = (%lx, %lx) \n",s,n);
    printf("cle privee = (%lx, %lx) \n",u,n);
    //chiffrement
    char mess[50]="hello";
    int len=strlen(mess);
    long *crypted=encrypt(mess,s,n);
    printf("Initial message %s \n",mess);
    printf("Encoded representation \n");
    print_long_vector(crypted,len);
    //dechiffrement
    char* decoded=decrypt(crypted,len,u,n);
    printf("Decoded: %s\n",decoded);
    Key* pKey=malloc(sizeof(Key));
    init_key(pKey, s,n);
    char* str=key_to_str(pKey);
    printf("%s\n",str);
    Key* sKey=str_to_key("(1273,1f15)");
    printf("%lu,%lu\n",pKey->cle,pKey->n);
    printf("%lu,%lu\n",sKey->cle,sKey->n);*/

    //Testing Init Keys
    /*Key* pKey = malloc(sizeof(Key));
    Key* sKey = malloc(sizeof(Key));
    init_pair_keys(pKey, sKey,3,7);
    printf("pKey: %lx , %lx \n", pKey->cle, pKey->n); 
    printf("sKey: %lx , %lx \n", sKey->cle, sKey->n);
    //Testing Key Serialization
    char* chaine = key_to_str(pKey);
    printf("key to str : %s \n", chaine);
    Key* k = str_to_key(chaine);
    printf("str to key : %lx , %lx \n", k->cle, k->n);
	free(k);
    //Testing signature
    //Candidate keys:
    Key* pKeyC = malloc(sizeof(Key));
    Key* sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC,3,7);
    //Declaration:
    char* mess = key_to_str(pKeyC);
    printf("%s vote pour %s\n",key_to_str(pKey), mess); 
    Signature* sgn = sign(mess, sKey);
    printf("signature : ");
    print_long_vector(sgn->content,sgn->size);
    chaine = signature_to_str(sgn);
    printf("signature to str : %s \n", chaine);
    sgn = str_to_signature(chaine);
    printf("str to signature : "); 
    print_long_vector(sgn->content,sgn->size);
    //Testing protected:
    Protected* pr = init_protected(pKey,mess, sgn);
    //Verification:
    if (verify(pr)){ 
        printf("Signature cleide\n");
    }
    else{
        printf("Signature non cleide\n");
    }
    chaine = protected_to_str(pr);
    printf(" protected to str : %s\n", chaine);
    //pr = str_to_protected(chaine);
    //printf("str to protected : %s %s %s\n",key_to_str(pr->pKey),pr->mess,signature_to_str(pr->sgn));
    free(pKey); 
    free(sKey); 
    free(pKeyC); 
    free(sKeyC);
    free(sgn);
    free(pr);
    free(mess);
	free(chaine);*/
    generate_random_data(50,10);
    return 0;

    //on obtient 215000363
}
