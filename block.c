#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"
#include "block.h"
#include <openssl/sha.h>

// question 7.1
void ecrire_block(char * filename, Block * block){
	/*fonction permettant d'écrire un bloc dans un fichier*/
    FILE *f = fopen(filename, "w");
    char *auth = key_to_str(block->author);
    fprintf(f, "%s\n", auth);
    free(auth);
    CellProtected *tmp = block->votes;
    while(tmp){
        char *pr = protected_to_str(tmp->data);

        fprintf(f, "%s\n", pr);

        free(pr);

        tmp = tmp->next;

    }
    fprintf(f, "%s\n", block->hash);
    fprintf(f, "%s\n", block->previous_hash);
    fprintf(f, "%d\n", block->nonce);
    fclose(f);
}
	
//question 7.2
Block* lire_block(char* filename){
	/* fonction permettant de lire un bloc depuis un fichier*/
    FILE *f = fopen(filename, "r");
    Block *block = (Block *)malloc(sizeof(Block));

    if (block == NULL){
        printf("Erreur lors de l'allocation du block\n");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    block->votes = create_cell_protected(NULL);
    CellProtected *droit = create_cell_protected(NULL);
    int nonce;

    if(fgets(buffer, 256, f)){
        block->author=str_to_key(buffer);
    }


    while(fgets(buffer, 256, f)){
        char *tmp=malloc(sizeof(char)*256);
        char *tmp1=malloc(sizeof(char)*120);
        char *tmp2=malloc(sizeof(char)*50);
        char *tmp3=malloc(sizeof(char)*100);
        
        if(sscanf(buffer, " %s %s %s\n", tmp1, tmp2, tmp3) == 3){
            strcpy(tmp, tmp1);
            strcat(tmp, " ");
            strcat(tmp, tmp2);
            strcat(tmp, " ");
            strcat(tmp, tmp3);
            strcat(tmp, "\n");
            
            Protected *pr = str_to_protected(tmp);
            block->votes = add_cell_protected(block->votes, pr);

        }
        else{
            free(tmp);
            block->hash=strdup(tmp1);
            free(tmp1);
            free(tmp2);
            free(tmp3);
            break;
        }

        free(tmp);
        free(tmp1);
        free(tmp2);
        free(tmp3);
    }

    if(fgets(buffer, 256, f)){
        buffer[strlen(buffer)-1]='\0';
        block->previous_hash=strdup(buffer);
    }
    
    if(fgets(buffer, 256, f)){    
        if(sscanf(buffer, "%d", &nonce) == 1){
            block->nonce=nonce;
        }
    }

    fclose(f);

    while(block->votes->next){
        droit=add_cell_protected(droit, block->votes->data);
        block->votes=block->votes->next;
        //printf("%s\n",protected_to_str(block->votes->data));
    }
    droit=add_cell_protected(droit, block->votes->data);
    //printf("%s\n",protected_to_str(droit->data));
    block->votes=droit;
    //delete_list_protected(droit);
    return block;
}

//question 7.3
char* block_to_str(Block* block){
	/*fonction qui génère une chaîne de caractères strblock 
        en fonction d'un block*/
    char* strblock = (char *)malloc(sizeof(char)*1000);
    char* tmp = key_to_str(block->author);
    strcpy(strblock, tmp);
    free(tmp);
    strcat(strblock, " ");
    strcat(strblock, block->previous_hash);
    
    strcat(strblock, " ");
    
    CellProtected *it = block->votes;
    while(it){

        char *pr = protected_to_str(it->data);
        strcat(strblock, pr);
        strcat(strblock, " ");
        free(pr);
        it = it->next;
    }

    char *c = malloc(sizeof(char)*10);
    sprintf(c, "%d\0", block->nonce);
    strcat(strblock, c);
    free(c);
    
    
    return strblock;

}

//question 7.5
char * hachage_SHA256(const unsigned char *s){
	/*fonction qui retourne la valeur hachée tmp d'une chaîne de caractères s
	en utilisant le hachage cryptographique SHA-256*/
	unsigned char *d = SHA256(s, strlen((const char*)s),0);
	
	char* tmp = malloc(sizeof(char)*strlen((const char*)d)*2+1);
	
	int i; 
	int cpt=0;
	for(i=0; i < SHA256_DIGEST_LENGTH; i++){     /* Returns the length in bytes of the hash value of the digest. */
		sprintf(&(tmp[cpt]), "%02x", d[i]);
		cpt=cpt+2;
	}
	
	tmp[cpt]='\0';
	return tmp;	
}

//question 7.6
void compute_proof_of_work(Block *B, int d){
	/*fonction permettant de rendre un bloc valide en procédant par brute force*/
    B->nonce = 0;
    int valide = 0;
    char* block;
    int a;
    unsigned char *tmp;
    while(B->nonce >= 0){
        a=0;
        block = block_to_str(B);
        tmp = str_to_hash(block);
        for (int i = 0; i<d; i++){//comptage du nb de zero
            if(tmp[i] != '0'){
                valide++;
                B->nonce++;
                free(tmp);
                free(block);
                a=1;
                break;
            }
        }
        if(valide == 0){//si d zero on sort de la boucle
            if(a==0){
                free(tmp);
                free(block);
                a=2;
            }
            break;
        }
        
        valide = 0; 
    }
    if(a==0){
        free(tmp);
        free(block);
        a=2;
    }
    
    /*
    for (int it = 0; it<SHA256_DIGEST_LENGTH; it++)
        printf("%c", tmp[it]);
    putchar('\n');
    */
    
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

//question 7.7
int verify_block(Block* b, int d){
    /* fonction permettant de vérifier la validité d'un bloc :
	retourne 1 si le block est valide, 0 sinon */
    char* block;
    unsigned char *tmp;
    block = block_to_str(b);
    tmp = str_to_hash(block);
 
    for (int i = 0; i<d; i++){
        if(tmp[i] != '0'){
            free(block);
            free(tmp);
            return 0;
        }
    }
    free(block);
    free(tmp);
    return 1;
}

//question 7.9
void delete_block(Block *b){
	/*supprime un bloc*/
    delete_list_protected(b->votes);
    free(b->author);
    free(b->hash);
    free(b->previous_hash);
    free(b);
    
}




