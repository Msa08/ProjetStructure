#ifndef BLOCKTREE_H
#define BLOCKTREE_H

#include "block.h"

typedef struct block_tree_cell{
    Block* block;
    struct block_tree_cell* father;
    struct block_tree_cell* firstChild;
    struct block_tree_cell* nextBro;
    int height;
}CellTree;

CellTree* create_node(Block* b);
int update_height(CellTree * father, CellTree * child);
void add_child(CellTree* father, CellTree* child);








#endif
