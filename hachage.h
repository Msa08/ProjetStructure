#ifndef HACHAGE_H
#define HACHAGE_H
#include "base.h"

typedef struct hashcell{
	Key* key;
	int val;
}HashCell;

typedef struct hashtable{
	HashCell ** tab;
	int size;
}HashTable;

HashCell* create_hashcell(Key* key);

int hash_function(Key* key, int size);

int find_position(HashTable* t, Key* key);

HashTable* create_hashtable(CellKey* keys, int size);

void delete_hashtable(HashTable* t); 

#endif
