#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "blocktree.h"

void submit_vote(Protected* p){
    FILE *f=fopen("Pending_votes.txt","a");
    char* pr=protected_to_str(p);
    fprintf(f,"%s\n",pr);
    free(pr);
    fclose(f);
}

void create_block(CellTree* tree, Key* author, int d){
    Block * b = (Block*) malloc(sizeof(Block));
    b->author = author;

    b->votes = read_protected("Pending_votes.txt");
    CellTree* last = last_node(tree);

    if (last == NULL){
        b->previous_hash = NULL;
    }
    else {
        b->previous_hash =(unsigned char*) strdup((const char*)last->block->hash);
    }
    //char* s_block = block_to_str(b) ?
    compute_proof_of_work(b, d);
    printf("fin compute\n");

    remove("Pending_votes.txt");
    
    printf("deb\n");
    ecrire_block("Pending_votes",b);
    printf("fin\n");

    delete_block(b);
}

void add_block(int d, char* name){
    Block* b = lire_block("Pending_block");
    if (verify_block(b, d) == 0){
        delete_block(b);
    }
    else{
        char nom_fichier[256];
        sprintf(nom_fichier,"/Blockchain/%s", name);
        ecrire_block("/Blockchain/%s",b);
        delete_block(b);
    }
    remove("Pending_block");
}

CellTree* read_tree(){
    CellTree** T = (CellTree**) malloc(sizeof(CellTree*));
    if (T == NULL){
        return NULL;
    }
    DIR* rep = opendir("/Blockchain/");
    //Etape 1
    int k=1;
    if (rep != NULL){
        struct dirent* dir;
        while ((dir = readdir(rep))){
            Block* b = lire_block(dir->d_name);
            if (k == 1){
                T[k-1] = create_node(b);
            }
            else{
                T = realloc(T, k*sizeof(CellTree*));
                T[k-1] = create_node(b);
            }
            k++;
        }
    }
    k--;
    //Etape 2
    for (int i = 0; i<k; k++){
        CellTree* father = T[i];
        for (int j=0; j<k; j++){
            CellTree* child = T[j];
            if (strcmp((const char*)father->block->hash,(const char*)child->block->previous_hash) == 0){
                child->father = father;
                add_child(father,child);
            }
        }
    }

    //Etape 3
    CellTree* racine;
    for (int i = 0; i<k; k++){
        if (T[i]->father == NULL){
            racine = T[i];
            break;
        }
    }
    free(T);
    return racine;
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
   
    CellProtected* declaration = fusion_highest_child(tree);
    filter(&declaration);
    return compute_winner(declaration, candidates, voters, sizeC, sizeV);
}