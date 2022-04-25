#ifndef READWRITE_H
#define READWRITE_H
#include "structure.h"
#include "chiffrement.h"
#include "struct.h"


CellKey *read_public_keys(char *nomFic);
CellProtected *read_protected(char *nomFichier);
void print_file_block(Block *block, char *name);
Block *read_file_block(char *nf);
void submit_vote(Protected* p);
void create_block(CellTree* tree, Key* author, int d);
void add_block(int d, char* name);
CellTree* read_tree();

#endif