#ifndef HACHAGE_H
#define HACHAGE_H
#include "Base.h"

typedef struct hashcell{
	Key* key;
	int val;
}HashCell;

typedef struct hashtable{
	HashCell ** tab;
	int size;
}HashTable;

void filter(CellProtected** LCP);

HashCell* create_hashcell(Key* key);

int hash_function(Key* key, int size);

int find_position(HashTable* t, Key* key);

HashTable* create_hashtable(CellKey* keys, int size);

void delete_hashtable(HashTable* t); 

Key* compute_winner(CellProtected* decl, CellKey* candidates,CellKey* voters, int sizeC, int sizeV);

void push_protected (CellProtected ** cp, Protected * pr);

CellProtected* cellProtectedDup(CellProtected * cp);

#endif
