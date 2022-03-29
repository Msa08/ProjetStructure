#ifndef BASE_H
#define BASE_H
#include "key.h"
#include "chiffrement.h"
#include "prime_number.h"

typedef struct cellKey{ 
    Key* data;
    struct cellKey* next; 
} CellKey;



CellKey* create_cell_key(Key* key);
void add_cell_key(CellKey** cellKey, Key* key);
CellKey* read_public_keys(char* fichier);
void print_list_keys(CellKey* LCK);

typedef struct cellProtected{ 
    Protected* data;
    struct cellProtected* next;
} CellProtected;
#endif