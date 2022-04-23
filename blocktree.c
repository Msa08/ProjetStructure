#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blocktree.h"

CellTree * create_node(Block* b){
    CellTree * res = (CellTree *) malloc(sizeof(CellTree));
    if (res == NULL){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    res->block = b;
    res->father = NULL;
    res->firstChild = NULL;
    res->nextBro = NULL;
    res->height= 0;

    return res;

}

int update_height(CellTree * father, CellTree * child){
    /* Retourne 0 si on a modifie la taille du pere.
    Retourne 1 sinon.
    */
    if (father->height > child->height+1){
        return 0;
    }
    father->height = child->height+1;
    return 1;
}

void add_child(CellTree* father, CellTree* child){
    /* Ajoute un fils à un noeud en mettant à jour la
    hauteur de tous les ascendants.
    */
    if (father->firstChild == NULL){
        father->firstChild = child;
        child->father = father;

    }
    else{
        child->father = father;
        child->nextBro = father->firstChild;
        father->firstChild = child;

        /*CellTree* courant = father->firstChild;
        while(courant->nextBro){
            courant = courant->nextBro;
        }
        courant->nextBro = child;
        */

    }

    while(father != NULL && update_height(father, child) == 1){
        child = father;
        father = father->father;
    }
}
