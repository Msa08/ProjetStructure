#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"
#include "block.h"
#include <openssl/sha.h>

void ecrire_block(char * filename, Block * block){
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
	

Block* lire_block(char* filename){
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
    return block;
}



/*char * block_to_str(Block* b){
	char * res = (char *) malloc(sizeof(char) * 1059 );
	char * author = key_to_str(b->author);
	char * previous_hash = strdup(b->previous_hash);
	char * vote;
    int i=0;

    if(!res){
        printf("probleme d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    for(int j=0;j < strlen(author);j++){
        res[i]=author[j];
        i++;
    }
    res[i]=' ';
    i++;

    for(int j=0; j < strlen(previous_hash); j++){
        res[i]=previous_hash[j];
        i++;
    }
    res[i]=' ';
    i++; 
	
	CellProtected * votes = b->votes;
	while (votes != NULL) {
		vote = protected_to_str(votes->data);
		for(int j=0; j < strlen(vote); j++){
            res[i]=vote[j];
            i++;
        }
        res[i]=' ';
        i++;
        votes=votes->next;
    }
    res[i]=
	//Libération mémoire
    free(author);
    free(previous_hash);
    free(vote);
	return res;
}*/

char* block_to_str(Block* block){
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


char * hachage_SHA256(const unsigned char *s){
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

void compute_proof_of_work(Block *B, int d){
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

int verify_block(Block* b, int d){
    // retourne 1 si le block est valide, 0 sinon
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
/*int verify_block(Block* b, int d){
    /* Verifie que le bloc b est correct.
    
    char* str_b = block_to_str(b);
    unsigned char* hash_value = hachage_SHA256(str_b);

    for (int i = 0; i < d; i++){
        if (hash_value[i] != '0'){
            free(str_b);
            return 0;
        }
    }
    if ((strcmp((const char*)b->hash, (const char*)hash_value)) != 0) {
        free(str_b);
        return 0;
    }
    free(str_b);
    return 1;
}*/


//qst7.9
void delete_block(Block *b){
	/*supprime un bloc*/
    delete_list_protected(b->votes);
    free(b->author);
    free(b->hash);
    free(b->previous_hash);
    free(b);
    
}




