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

    if(!father){ // test si arbre vide, si oui le crée
        father = create_node(child->block);
        printf("Il n'y a pas de pere\n");
        return;
    }

    CellTree *debut=father; 
    if (!father->firstChild){ // test si le noeud n'a pas de fils
        father->firstChild = child;
        child->block->previous_hash = father->block->hash;
        child->father=father;
        
        CellTree *it = father;
        CellTree *itsuiv = child;

        while(it){// update de la taille des noeuds
            update_height(it, itsuiv);
            it=it->father;
            itsuiv=itsuiv->father;
        }
        father=debut;
        return;
    }

    CellTree *it = father->firstChild;
    while(it->nextBro){
            it=it->nextBro;
    }
    it->nextBro = child;
    child->block->previous_hash = father->block->hash;
    child->father=father;
    
    CellTree *itr = father;
    CellTree *itrsuiv = child;
    while(itr){
        update_height(father, child);
        itr=itr->father;
        itrsuiv=itrsuiv->father;
    }

    father=debut;
}


void print_tree(CellTree* node){
    if(!node || !node->block){
        printf("Arbre vide\n");
        return;
    }
    printf("Hauteur = %d , Hash = %s\n", node->height, node->block->hash);
    printf("Previous_hash = %s\n", node->block->previous_hash);
    printf("\n");

    CellTree *cour=node->firstChild;
    while(cour){
        print_tree(cour);
        cour=cour->nextBro;
    }   
}

// // la notre
// void delete_node(CellTree* node){
//     delete_block(node->block);
//     free(node);
// }

//la leur
void delete_node(CellTree* node){
    delete_block(node->block);

    if(node->father)
        free(node->father);
    if(node->firstChild)
        free(node->firstChild);
    if(node->nextBro)
        free(node->nextBro);
    
    free(node);
}
//la notre
void delete_tree(CellTree* tree){
    if (tree == NULL){
        return;
    }
    delete_tree(tree->firstChild);
    delete_tree(tree->nextBro);

    delete_node(tree);
}

// //la leur
// void delete_tree(CellTree* node){
//     if(!node->block){
//         printf("Arbre vide\n");
//         return;
//     }

//     CellTree *cour=node->firstChild;
//     while(cour){
//         if(!cour->nextBro){
//             delete_node(cour); 
//             return;
//         }
            
//         delete_tree(cour);
//         cour=cour->nextBro;
//     }
// }

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

/*
CellProtected* fusion_highest_child(CellTree * tree){
  CellTree * tmp = highest_child(tree);
  CellProtected * res = cellProtectedDup(tmp->block->votes);
  tmp = tmp->firstChild;
  while (tmp) {
    res = fusion_declaration(res, tmp->block->votes);
    tmp = tmp->firstChild;
  }
  return res;
}
*/

// AUTRE VERSION 
CellProtected* fusion_highest_child(CellTree * tree) {
  // Arbre vide
  if (tree == NULL) {
    return NULL;
  }


  // Ajout liste du premier noeud
  CellProtected* list = NULL;
  merge_list_protected(&list, &((tree->block)->votes));


  // Ajout liste des fils
  CellTree* highestChild = highest_child(tree);
  while (highestChild != NULL) {
    merge_list_protected(&list, &((highestChild->block)->votes));
    highestChild = highest_child(highestChild);
  }

  return list;
}

