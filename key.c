#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "key.h"
#include "prime_number.h"
#include "chiffrement.h"
#include "math.h"
void init_key(Key* key, long val, long n){
    key->n=n;
    key->cle=val;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
    long a=random_prime_number(low_size,up_size,5000);
    long b=random_prime_number(low_size,up_size,5000);
    while(a==b){
        b=random_prime_number(low_size,up_size,5000);
    }
    long n=0;
    long u=0;
    long s=0;
    generate_key_values(a,b, &n, &s, &u);
    if(u<0){
        long t= (a-1)*(b-1);
        u=u+t;
    }
    pKey->cle=s;
    pKey->n=n;
    sKey->cle=u;
    sKey->n=n;
}

char* key_to_str(Key* key){
    char* str=malloc(20*sizeof(char));
    int i=0;
    long a=key->cle;
    long b=key->n;
    sprintf(str,"(%lx,%lx)",a,b);
    return str;
}

Key* str_to_key(char* str){
    Key* pKey=malloc(sizeof(Key));
    long cle, n;
    sscanf(str,"(%lx,%lx)",&cle,&n);
    init_key(pKey,cle,n);
    return pKey;
}

Signature* init_signature(long* content, int size){
    Signature* signature=malloc(sizeof(Signature));
    signature->size=size;
    signature->content=content;
    return signature;
}

Signature* sign(char* mess, Key* sKey){
    long* tab=encrypt(mess,sKey->cle,sKey->n);
    Signature* signature=init_signature(tab, strlen(mess));
    return signature;
}

char* signature_to_str(Signature* sgn){
    char* result = malloc(10*sgn->size*sizeof(char)); 
    result[0]= '#';
    int pos = 1;
    char buffer[156];
    for(int i=0;i<sgn->size;i++){
        sprintf(buffer,"%lx",sgn->content[i]);
        for(int j=0;j<strlen(buffer);j++){
            result[pos]=buffer[j];
            pos+=1;
        }
        result[pos]='#';
        pos+=1;
    }
    result[pos]='\0';
    result = realloc(result, (pos+1)*sizeof(char));
    return result;
}

Signature* str_to_signature(char* str){
    int len = strlen(str);
    long* content = (long*)malloc(sizeof(long)*len); 
    int num = 0;
    char buffer [256];
    int pos = 0;
    for (int i=0; i<len; i++){
        if (str[i] != '#'){ 
            buffer[pos] = str[i]; 
            pos=pos+1;
        }
        else{
            if (pos != 0){
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num])); 
                num = num + 1;
                pos = 0;
            } 
        }
    }
    content=realloc(content ,num*sizeof(long)); 
    return init_signature(content , num);
}

Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
    Protected* protected=malloc(sizeof(Protected));
    protected->pKey=pKey;
    protected->mess=mess;
    protected->sgn=sgn;
    return protected;
}

int verify(Protected* pr){
    Key* pKey=pr->pKey;
    char* mess=pr->mess;
    Signature* sgn=pr->sgn;
    if(strcmp(decrypt(sgn->content,sgn->size,pKey->cle,pKey->n),mess)==0){
        return 1;
    }
    return 0;
}

char* protected_to_str(Protected* pr){
    char* key=key_to_str(pr->pKey);
    char* sgn=signature_to_str(pr->sgn);
    char* str=malloc((strlen(key)+strlen(sgn)+strlen(pr->mess))*sizeof(char));
    sprintf(str,"%s %s %s",key, pr->mess, sgn);
    return str;
}

/*Protected* str_to_protected(char* str){
    Protected* pr=malloc(sizeof(Protected));
    char* key=malloc(10*sizeof(char));
    int i=0;
    while(str!=' '){
        key[i]=str[i];
        i++;
        str++;
    }
    i++;
    key[i]='\0';
    char* mess
    while(str!=' '){

    }
}*/

void generate_random_data(int nv, int nc){
    srand(time(NULL));
    FILE *f=fopen("keys.txt","w");
    FILE *f2=fopen("candidates.txt","w");
    if(f==NULL || f2==NULL){
        printf("erreur allocation\n");
        return;
    }
    Key* pKey=malloc(sizeof(Key));
    Key* sKey=malloc(sizeof(Key));
    if(pKey==NULL || sKey==NULL){
        printf("erreur allocation\n");
        return;
    }
    Signature *signature;
    for(int i=0;i<nv;i++){
        init_pair_keys(pKey,sKey,3,12);
        fprintf(f,"%s %s\n",key_to_str(pKey),key_to_str(sKey));
    }
    fclose(f);
    FILE *f4=fopen("keys.txt","r");
    char* pKey2=malloc(61*sizeof(char));
    char *ligne=malloc(60*sizeof(char));
    char* poubelle=malloc(60*sizeof(char));
    long j;
    int i=0;
    Key* tab[nc];
    int a=0;
    while(i<nc){
        j=rand_long(1,nv);
        printf("j=%ld i=%d\n",j,i);
        rewind(f4);
        for(int h=0;h<j;h++){
            fgets(ligne,60,f4);
        }
        printf("ligne : %s\n",ligne);
        sscanf(ligne,"%s %s",pKey2, poubelle);
        for(int z=0;z<nc;z++){
            if((tab[z]->cle==str_to_key(pKey2)->cle) && (tab[z]->n==str_to_key(pKey2)->n)){
                printf("tab[%d]->cle=%lx et tab[%d]->n=%lx\n",z,tab[z]->cle,z,tab[z]->n);
                printf("key : %s\n",pKey2);
                printf("break\n");
                a=1;
                break;
            }
        }
        if(a==1){
            a=0;
            printf("continue\n"); 
            continue;
        }
        else{
            fprintf(f2,"%s\n",pKey2);
            tab[i]=str_to_key(pKey2);
            printf("1EEEEELLLLSSSSSSEEEEEEE\n");
            i++;
        }
    }

    fclose(f2);
    fclose(f4);
    FILE *f6=fopen("keys.txt","r");
    FILE *f3=fopen("declarations.txt","w");
    FILE *f5=fopen("candidates.txt","r");
    char* ligne2=malloc(60*sizeof(char));
    
    for(int i=0;i<nv;i++){
        fgets(ligne2,50,f6);
        j=rand_long(1,nc);
        for(int h=0;h<j;h++){
            fgets(ligne,50,f5);
        }
        sscanf(ligne2,"(%s,%s)",poubelle, pKey2);
        pKey=str_to_key(pKey2);
        signature=sign(ligne,str_to_key(ligne2));
        Protected *pr=init_protected(pKey,ligne,signature);
        fprintf(f3,"%s\n",protected_to_str(pr));
    }
    fclose(f);
    fclose(f4);
    fclose(f5);
    fclose(f6);
    free(ligne);
    free(ligne2);
    free(pKey);
    free(sKey);
}