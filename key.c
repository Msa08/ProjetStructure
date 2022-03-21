#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "key.h"
#include "prime_number.h"
#include "chiffrement.h"
#include "math.h"
void init_key(Key* key, long val, long n){
    key->n=n;
    key->clé=val;
}

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
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
    sprintf(str,"(%lx,%lx)",a,b);
    return str;
}

Key* str_to_key(char* str){
    Key* pKey=malloc(sizeof(Key));
    long clé, n;
    sscanf(str,"(%lx,%lx)",&clé,&n);
    init_key(pKey,clé,n);
    return pKey;
}

Signature* init_signature(long* content, int size){
    Signature* signature=malloc(sizeof(Signature));
    signature->size=size;
    signature->content=content;
    return signature;
}

Signature* sign(char* mess, Key* sKey){
    long* tab=encrypt(mess,sKey->clé,sKey->n);
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
    if(strcmp(decrypt(sgn->content,sgn->size,pKey->clé,pKey->n),mess)==0){
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