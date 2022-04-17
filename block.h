#ifndef BLOCK_H
#define BLOCK_H
#include "hachage.h"


typedef struct block {
	Key * author ;
	CellProtected * votes ;
	unsigned char * hash ;
	unsigned char * previous_hash ;
	int nonce ;
} Block ;






void ecrire_block(char * filename, Block * b);

Block* lire_block(char* filename);

char * block_to_str(Block* b);

char * hachage_SHA256(const unsigned char * s);
