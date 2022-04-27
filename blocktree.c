#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blocktree.h"


//question 8.1
CellTree * create_node(Block* b){
    /*fonction qui permet de créer et initialiser un noeud avec une hauteur à 0*/
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

//question 8.2
int update_height(CellTree * father, CellTree * child){
    /*Fonction permettant de màj la hauteur du noeud father quand l'un de ses fils a été modifié
    Retourne 0 si on a modifie la taille du pere.
    Retourne 1 sinon.*/
    
    if (father->height > child->height+1){
        return 0;
    }
    father->height = child->height+1;
    return 1;
}

//question 8.3
void add_child(CellTree* father, CellTree* child){
    /*fonction qui ajoute un fils à un noeud*/
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

//question 8.4
void print_tree(CellTree* node){
    /*fonction qui affiche arbre + la hauteur de chaque noeud et la valeur hachée du bloc*/
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

//question 8.5
void delete_node(CellTree* node){
    /*fonction qui supprime un noeud de l'arbre*/
    delete_block(node->block);

    if(node->father)
        free(node->father);
    if(node->firstChild)
        free(node->firstChild);
    if(node->nextBro)
        free(node->nextBro);
    
    free(node);
}
//fonction moins optimisée que delete_node
void delete_tree(CellTree* tree){
    if (tree == NULL){
        return;
    }
    delete_tree(tree->firstChild);
    delete_tree(tree->nextBro);

    delete_node(tree);
}

//question 8.6
CellTree* highest_child(CellTree* cell){
    /*fonction qui renvoie le noeud fils avec la plus grande hauteur*/
    if (cell == NULL){
        return NULL;
    }
    CellTree* filsCourant = cell->firstChild;
    while (filsCourant != NULL && filsCourant->height != cell->height-1 ){
        filsCourant = filsCourant->nextBro;
    }
    return filsCourant;
}

//question 8.7
CellTree* last_node(CellTree* cell){
    /*fonction qui retourne la valeur hachée du dernier bloc de cette plus longue chaîne*/
    CellTree* filsCourant = highest_child(cell);
    if (filsCourant == NULL){
        return cell;
    }

    while (filsCourant->firstChild != NULL) {
        filsCourant = highest_child(filsCourant);
    }
    return filsCourant;
}

//question 8.8
CellProtected* fusion_declaration(CellProtected* cell1, CellProtected* cell2){
    /*fonction permettant de fusionner deux listes chainées de déclarations signées*/
    if (cell1 == NULL){
        return cell2;
    }
    while(cell1->next){
        cell1 = cell1->next;
    }
    cell1->next = cell2;
    return cell1;
}

// question 8.9 + une variante

CellProtected* fusion_highest_child(CellTree * tree) {
    /*fonction qui retourne la liste obtenue par fusion des listes chaînées*/
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

// AUTRE VERSION
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
