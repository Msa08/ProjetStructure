#include <stdio.h>
#include <stdlib.h>
#include "prime_number.h"
#include "chiffrement.h"
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
    long a=random_prime_number(3,7,5000);
    long b=random_prime_number(3,7,5000);
    while(a==b){
        b=random_prime_number(3,7,5000);
    }
    printf("nb premier %lu %lu\n",a,b);

    long n;
    long u;
    long s;
    printf("bonjour\n");
    generate_key_values(a,b, &n, &s, &u);
    printf("bjr\n");
    if(u<0){
        long t= (a-1)*(b-1);
        u=u+t;
    }

    //affichage des clés en haxadécimal
    printf("cle publique = (%lx, %lx) \n",s,n);
    printf("cle privee = (%lx, %lx) \n",u,n);
    //chiffrement
    char mess[10]="hello";
    int len=strlen(mess);
    long *crypted=encrypt(mess,s,n);
    printf("Initial message %s \n",mess);
    printf("Encoded representation \n");
    print_long_vector(crypted,len);
    //dechiffrement
    char* decoded=decrypt(crypted,len,u,n);
    printf("Decoded: %s\n",decoded);
    return 0;
    //on obtient 215000363
}
