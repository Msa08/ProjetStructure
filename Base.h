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
CellKey* add_cell_key(CellKey* cellkey, Key* key);
CellKey *read_public_keys(char *nomFic);
void print_list_keys(CellKey* LCK);
void delete_list_keys(CellKey* LCK);
void delete_cell_key(CellKey* c);

typedef struct cellProtected{ 
    Protected* data;
    struct cellProtected* next;
} CellProtected;

CellProtected* create_cell_protected(Protected* pr);
CellProtected* add_cell_protected(CellProtected *cellpr, Protected *pr);
CellProtected *read_protected(char *nomFichier);
void print_list_protected(CellProtected* LCP);
void delete_cell_protected(CellProtected* cp);
void delete_list_protected(CellProtected* LCP);
void deleted_invalid_sign(CellProtected *LCP);
#endif