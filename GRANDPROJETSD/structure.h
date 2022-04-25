#ifndef STRUCTURE_H
#define STRUCTURE_H
#include "chiffrement.h"
#include "struct.h"


void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);
void print_list_keys(CellKey* LCK);
Signature* init_signature(long* content, int size);
Protected* init_protected(Key* pKey, char* mess, Signature* sgn);
CellKey* create_cell_key(Key* key);
CellKey* ajout_en_tete_CellKey(CellKey* cellkey, Key* key);
void delete_cell_key(CellKey* c);
void delete_list_keys(CellKey* c);
CellProtected* create_cell_protected(Protected* pr);
CellProtected* ajout_en_tete_CellPR(CellProtected *cellpr, Protected *pr);
void afficher_cellPR(CellProtected* cellpr);
void delete_cell_protected(CellProtected* c);
void delete_list_protected(CellProtected* c);
void delete_fraud_list_protected(CellProtected** c);
HashCell* create_hashcell(Key* key);
int find_position(HashTable* t, Key* key);
HashTable* create_hashtable(CellKey* keys, int size);
void delete_hashtable(HashTable* t);
void delete_block(Block* b);
CellTree* create_node(Block* b);
int update_height(CellTree* father, CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_tree(CellTree* node);
void delete_node(CellTree* node);
void delete_tree(CellTree* node);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
CellProtected *fusion(CellProtected *liste1, CellProtected* liste2);
CellProtected *fusion_decl(CellTree *tree);

int verify_block(Block* b, int d);
int verify(Protected* pr);

char* key_to_str(Key* key);
Key* str_to_key(char* str);
char *signature_to_str(Signature *sgn);
Signature *str_to_signature(char * str);
char *protected_to_str(Protected *pr);
Protected* str_to_protected(char *str);
char* block_to_str(Block* block);


#endif