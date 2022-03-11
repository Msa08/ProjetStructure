#include <stdio.h>
#include <stdlib.h>
#include "prime_number.h"
#include "chiffrement.h"
#include <time.h>

int main(){
    FILE *f = fopen("modpow.txt", "w");
    FILE *f2=fopen("modpow2.txt", "w");
    clock_t time_initial=0;
    clock_t time_final=0;
    clock_t time_initial2=0;
    clock_t time_final2=0;
    double time;
    double time2;
    /*long i;
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
    return 0 ;*/

    /*for(int i=1;i<1000000;i++){
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

    long a=random_prime_number(8,127,5000);
    long b=random_prime_number(8,127,5000);
    printf("nb premier %lu %lu\n",a,b);
    long tts = 0;
    long ttn = 0;
    long ttu = 0;
    long *n=&ttn;
    long *u=&ttu;
    long *s=&ttn;
    generate_key_values(a,b, n, s, u);
    printf("n:%lu s:%lu u:%lu\n",*n,*s,*u);
    return 0;
    //on obtient 215000363
}
