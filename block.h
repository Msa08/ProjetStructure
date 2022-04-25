<<<<<<< HEAD
#ifndef BLOCK_H
#define BLOCK_H
#include "hachage.h"
#include <openssl/sha.h>

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

void compute_proof_of_work(Block* b, int d);

int verify_block(Block* b, int d);


#endif
=======
#ifndef BLOCK_H
#define BLOCK_H
#include "hachage.h"
#include <openssl/sha.h>

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

void compute_proof_of_work(Block* b, int d);

int verify_block(Block* b, int d);

void delete_block(Block *b);
	
#endif
>>>>>>> 69701f70dca74916819c95df83e9ad54ccea4d33
