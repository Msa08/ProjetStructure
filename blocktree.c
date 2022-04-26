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
    Retourne 1 sinon.*/
    
    if (father->height > child->height+1){
        return 0;
    }
    father->height = child->height+1;
    return 1;
}

void add_child(CellTree* father, CellTree* child){
    /* Ajoute un fils à un noeud en mettant à jour la
    hauteur de tous les ascendants.*/
    
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
        courant->nextBro = child;*/
        

    }

    while(father != NULL && update_height(father, child) == 1){
        child = father;
        father = father->father;
    }
}

void print_tree(CellTree * tree){
    /* Affiche l'arbre : pour chaques noeuds, la hauteur du noeud et la valeur hachee du bloc
       son affiche.*/
    

    if (tree == NULL){
        return;
    }
    CellTree* courant = tree;
    while (courant) {
        printf("Hauteur = %d , Hash = %s\n", courant->height, courant->block->hash);
        courant = courant->nextBro;
    }
    printf("\n");

    print_tree(tree->firstChild);
}


void delete_node(CellTree* node){
    delete_block(node->block);
    free(node);
}

void delete_tree(CellTree* tree){
    if (tree == NULL){
        return;
    }
    delete_tree(tree->firstChild);
    delete_tree(tree->nextBro);

    delete_node(tree);
}
CellTree* highest_child(CellTree* cell){
    if (cell == NULL){
        return NULL;
    }
    CellTree* filsCourant = cell->firstChild;
    while (filsCourant != NULL && filsCourant->height != cell->height-1 ){
        filsCourant = filsCourant->nextBro;
    }
    return filsCourant;
}

CellTree* last_node(CellTree* cell){
    CellTree* filsCourant = highest_child(cell);
    if (filsCourant == NULL){
        return cell;
    }

    while (filsCourant->firstChild != NULL) {
        filsCourant = highest_child(filsCourant);
    }
    return filsCourant;
}

CellProtected* fusion_declaration(CellProtected* cell1, CellProtected* cell2){
    if (cell1 == NULL){
        return cell2;
    }
    while(cell1->next){
        cell1 = cell1->next;
    }
    cell1->next = cell2;
    return cell1;
}

// 8.9 à tester 

CellProtected * fusion_highest_child(CellTree * tree){
  CellTree * tmp = highest_child(tree);
  CellProtected * res = cellProtectedDup(tmp->block->votes);
  tmp = tmp->firstChild;
  while (tmp) {
    res = fusion_declaration(res, tmp->block->votes);
    tmp = tmp->firstChild;
  }
  return res;
}

