#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "key.h"
#include "prime_number.h"
#include "chiffrement.h"
#include "math.h"

//question 3.2
void init_key(Key* key, long val, long n){
    /*fonction permettant d'initialiser une clé key déja allouée*/
    key->n=n;
    key->cle=val;
}

//question 3.3
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
    /*fonction permettant d'initialiser une clé publique et une clé secrète déja allouées*/
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

//question 3.4
char* key_to_str(Key* key){
    /*fonction permettant de passer d'une variable de type Key à sa valeur en str*/
    char* str=malloc(50*sizeof(char));
    long a=key->cle;
    long b=key->n;
    sprintf(str,"(%lx,%lx)",a,b);
    return str;
}
//question 3.4 
Key* str_to_key(char* str){
    /*fonction permettant de passer d'une variable de type str au type Key*/
    Key* pKey=malloc(sizeof(Key));
    long cle, n;
    sscanf(str,"(%lx,%lx)",&cle,&n);
    init_key(pKey,cle,n);
    return pKey;
}

//question 3.6
Signature* init_signature(long* content, int size){
    /*fonction permettant d'allouer et de remplir une signature avec un tableau content de type long*/
    Signature* signature=malloc(sizeof(Signature));
    signature->size=size;
    signature->content=content;
    return signature;
}

//question 3.7
Signature* sign(char* mess, Key* sKey){
    /*fonction permettant de créer une signature à partir de mess et de la clé sKey*/
    long* tab=encrypt(mess,sKey->cle,sKey->n);
    Signature* signature=init_signature(tab, strlen(mess));
    return signature;
}

//question 3.8
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
//question 3.8
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

//question 3.10
Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
    /*fonction qui alloue et initialise une structure Protected*/
    Protected* protected=malloc(sizeof(Protected));
    protected->pKey=pKey;
    protected->mess=mess;
    protected->sgn=sgn;
    return protected;
}

//question 3.11
int verify(Protected* pr){
    /*fonction qui vérifie que la signature correspond au message et à la personne contenue dans pr,
    renvoie 1 si c'est valide sinon 0*/
    Key* pKey=pr->pKey;
    char* mess=pr->mess;
    Signature* sgn=pr->sgn;
    char* decrypt1=decrypt(sgn->content,sgn->size,pKey->cle,pKey->n);
    if(strcmp(decrypt1,mess)==0){
        free(decrypt1);
        return 1;
    }
    free(decrypt1);
    return 0;
}

//question 3.12
char* protected_to_str(Protected* pr){
    /*fonction permettant de passer d'une structure Protected à sa valeur en str*/
    char* key=key_to_str(pr->pKey);
    char* sgn=signature_to_str(pr->sgn);
    char* str=malloc(2*(strlen(key)+strlen(sgn)+strlen(pr->mess))*sizeof(char));
    sprintf(str,"%s %s %s",key, pr->mess, sgn);
    free(key);
    free(sgn);
    return str;
}

//question 3.12
Protected* str_to_protected(char* str){
    /*fonction permettant de passer d'une str à une structure Protected*/
   Protected* pr=malloc(sizeof(Protected));
     char key[255];
     char mess[255];
     char signature[255];
     sscanf(str,"%s %s %s",key,mess,signature);
     pr->pKey=str_to_key(key);
     pr->mess=strdup(mess);
     pr->sgn=str_to_signature(signature);
     return pr;
}



//EXERCICE 4
//question 4.1
void generate_random_data(int nv, int nc){
    /*fonction réalisant une simulation de vote aléatoire*/

    Key *pKey = NULL;
    Key *sKey = NULL; 
    char* pKeyc;
    char* sKeyc;

    FILE *f = fopen("keys.txt", "w");

    // on genere des cles et on ajoute leur representation caractere dans le fichier <keys.txt>
    for (int i=0; i<nv; i++){
        pKey = (Key*)malloc(sizeof(Key));
        sKey = (Key*)malloc(sizeof(Key)); 
        init_pair_keys(pKey, sKey, 3, 7);
        pKeyc = key_to_str(pKey);
        sKeyc = key_to_str(sKey),
        fprintf(f, "%s %s\n", pKeyc, sKeyc);
        free(pKey);
        free(sKey);
        free(pKeyc);
        free(sKeyc);
    }
    fclose(f);

    // RECUPERE les cles publiques dans le fichier Keys 
    // et les ajoute dans le fichier CANDIDATES

    FILE *fk = fopen("keys.txt", "r");
    FILE *fc = fopen("candidates.txt", "w");

    //int rand = rand_long(1, nv);
    char **tabCandidats = (char**)malloc(sizeof(char*)*nc);
    for (int i =0; i<nc; i++){
        tabCandidats[i]=malloc(150*sizeof(char));
    }

    char buffer[250];
    char str[150];
    int it = 0;
    int irand;
    int *tabRand = (int*)malloc(sizeof(int)*nc);
    int itr = 0;
    int estDedans = 0;

    while(it<nc){
        irand = rand() % nv;
        while(itr<it){
            if(tabRand[itr] == irand){
                estDedans++;
                break;
            }
            itr++;
        }
        if(estDedans == 0){
            tabRand[it] = irand; 
            it++;
        }
        itr = 0;
        estDedans = 0;
        irand = 0;

    }

    int tmp;
    for(int i = 0; i < nc; ++i){
        for(int j = i + 1; j < nc; ++j){
            if(tabRand[i] > tabRand[j]){
                tmp = tabRand[i];
                tabRand[i] = tabRand[j];
                tabRand[j] = tmp;
            }
        }
    }
   
    it = 0;
    itr = 0;
    while(fgets(buffer, 100, fk) && itr<nc){
        if(it == tabRand[itr]){
            if (sscanf(buffer, " %s\n", str)==1){
                strcpy(tabCandidats[itr], str);
                fprintf(fc, "%s\n", tabCandidats[itr]);
            }
            itr++;
        }
        it++;
    }

    fclose(fk);
    fclose(fc);

    // GENERE les declarations de vote (a partir de Keys) et les
    // enregistre dans un fichier Declaration.txt

    FILE *fkr = fopen("keys.txt", "r");
    FILE *fs = fopen("declarations.txt", "w");
    char str2[100];
    char str3[100];
    char* signature;
    Signature* sgn;
    while(fgets(buffer, 256, fkr)){
        if (sscanf(buffer, " %s %s \n", str2, str3)==2){
            sKey = str_to_key(str3);
            long rand = rand_long(0, nc-1);
            sgn=sign(tabCandidats[rand], sKey);
            signature=signature_to_str(sgn);
            fprintf(fs, "%s %s %s \n", str2, tabCandidats[rand], signature);
            free(sKey);
            free(sgn->content);
            free(sgn);
            free(signature);
        }
    }
    fclose(fkr);
    fclose(fs);
    printf("t\n");
    for(int i=0; i<nc;i++){
         free(tabCandidats[i]);
    }   
    free(tabCandidats);
    free(tabRand);  
    
}
