#include "block.h"

void ecrire_block(char * filename, Block * b){
	FILE* f = fopen(filename, "w");
	
	char* author = key_to_str(b->author);
	fprintf(f, "%s\n%s\n%s\n%d\n\n",author, b->hash, b->previous_hash, b->nonce);
	
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
