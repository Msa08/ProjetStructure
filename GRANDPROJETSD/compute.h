#ifndef COMPUTE_H
#define COMPUTE_H
#include "structure.h"
#include "chiffrement.h"
#include "readwrite.h"
#include "struct.h"

void generate_random_data(int nv, int nc);
Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);
void compute_proof_of_work(Block *B, int d);
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif