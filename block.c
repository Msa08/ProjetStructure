#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"
#include "Block.h"

void ecrire_block(char * filename, Block * b){
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
}
	


Block* lire_block(char* filename){
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
}

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

