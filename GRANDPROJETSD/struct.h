#ifndef STRUCT_H
#define STRUCT_H

typedef struct _key{
	long val;
    long n;
} Key;

typedef struct _signature{
    long *content;
    int size;
} Signature;

typedef struct _protected{
    Key *pKey;
    char *mess;
    Signature *sgn;
} Protected;

typedef struct cellKey {
    Key *data;
    struct cellKey *next;
} CellKey;

typedef struct cellProtected {
    Protected *data;
    struct cellProtected *next;
} CellProtected;

typedef struct hashcell {
    Key *key;
    int val;
} HashCell;

typedef struct hashtable {
    HashCell **tab;
    int size;
} HashTable;

typedef struct block {
    Key *author;
    CellProtected *votes;
    unsigned char *hash;
    unsigned char *previous_hash;
    int nonce;
} Block;

typedef struct block_tree_cell {
    Block *block;
    struct block_tree_cell *father;
    struct block_tree_cell *firstChild;
    struct block_tree_cell *nextBro;
    int height;
} CellTree;



#endif