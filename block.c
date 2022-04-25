#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"
#include "Block.h"
//kacem
/*void ecrire_block(char * filename, Block * b){
	FILE* f = fopen(filename, "w");
	
	char* author = key_to_str(b->author);
	fprintf(f, "%s\n%s\n%s\n%d\n\n",author, b->hash, b->previous_hash, b->nonce);
	
	if (b == NULL){
        printf("Bloc nul dont écriture impossible!\n");
		return;
	}
	
	while(b->votes != NULL){
		char * vote = protected_to_str(b->votes->data);
		fprintf(f, "%s\n", vote);
		free(vote);
		
		b->votes = b->votes->next;
	}
}*/


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
    }
    delete_cell_protected(block->votes);
    block->votes=droit;
    return block;
}
//kacem 
/*Block* lire_block(char* filename){
	FILE* f = fopen(filename, "r");
	
	Block* b=malloc(sizeof(Block));
	
	char buffer[256] = "";
	char author[50];
	unsigned char hash[50];
	unsigned char p_hash[50];
	char vote[50];
	int nonce;
	
	fgets(buffer, sizeof(buffer), f);
	sscanf(buffer, "%s\n", author);
	Key* key_author = str_to_key(author);
	
	fgets(buffer, sizeof(buffer), f);
	sscanf(buffer, "%s\n", hash);
	
	fgets(buffer, sizeof(buffer), f);
	sscanf(buffer, "%s\n", p_hash);
	
	fgets(buffer, sizeof(buffer), f);
	sscanf(buffer, "%d\n", &nonce);
	
	fgets(buffer, sizeof(buffer), f);
	sscanf(buffer, "%s\n", vote);
	Protected* pr = str_to_protected(vote);
	CellProtected* cpr = create_cell_protected(pr);
	
	while(fgets(buffer, sizeof(buffer), f)){
		sscanf(buffer, "%s\n", vote);
		Protected* pr = str_to_protected(vote);
		add_cell_protected(cpr, pr);
	}
	
	b->author = key_author;
	b->hash = hash;
	b->previous_hash = p_hash;
	b->votes = cpr;
	
	return b;
}*/

char * block_to_str(Block* b){
	char strb[1000];

	char* author = key_to_str(b->author);
	sprintf(strb, "%s\n%s\n\n%s\n%d\n\n", author, b->hash, b->previous_hash, b->nonce);
	
	while(b->votes != NULL){
		char * vote = protected_to_str(b->votes->data);
		sprintf(strb, "%s\n", vote);
		b->votes = b->votes->next;
	}
	
	return strdup(strb);
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

void compute_proof_of_work(Block* b, int d){
	int cpt = 0;
	for(int i=0; i<strlen((const char*)b->hash); i++){
		if(cpt == d){
			return;
		}
		if(b->hash[i] != '0'){
			cpt=0;
		}
		if (b->hash[i]=='0'){
			cpt++;
		}
		(b->nonce)++;
	}
}

/*int verify_block(Block* b, int d){
	if(b->nonce == d){
		return 1;
	}
	return 0;
}

*/
int verify_block(Block* b, int d){
    /* Verifie que le bloc b est correct.
    */
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
}

