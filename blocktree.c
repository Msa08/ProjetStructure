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

void print_tree(CellTree * tree){
    /* Affiche l'arbre : pour chaques noeuds, la hauteur du noeud et la valeur hachee du bloc
       son affiche.
    */

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

/*
/=========================== Question 8.1 ======================================/

CellTree* create_node(Block* b){
    //permet de creer et initialiser un noeud avec une hauteur egale a zero.
    CellTree * res = (CellTree *)malloc(sizeof(CellTree));
    res->block = b;
    res->height = 0;
    res->father = NULL;
    res->firstChild = NULL;
    res->nextBro = NULL;
    return res;
}
/=========================== Question 8.2 ======================================/

int maxh(int a, int b){
    if (a > b){ 
        return a;
    }
    return b; 
}

int hauteur(CellTree* node){
    return node->height;
}

int update_height(CellTree* father, CellTree* child){
    //fonction permettant de mettre a jour la hauteur du noeud father quand l'un de ses fils a ete modifie : la hauteur du noeud father doit etre egale au max 
    //entre sa hauteur courante et la hauteur du noeud son + 1.Cette fonction retourner 1 si la hauteur du noeud father a change, et 0 sinon.
    if(!father || !child){
        return -1; 
    }
    //child->father == father;
    int h;
    h = hauteur(child)+1;
    father->height = maxh(hauteur(father),h);
    if(father->height != h){
        return 0;
    }
    return 1;
}
/=========================== Question 8.3 ======================================/

void add_child(CellTree* father, CellTree* child){
    //fonction qui ajoute un fils a un noeud en mettant a jour la hauteur de tous les ascendants

   //si le fils n'a pas de pere
    if (father == NULL){
       father = child;
       return;
    }

    child->father=father;
    CellTree* tmp_child = father->firstChild;
    if(tmp_child == NULL){
        father->firstChild = child;
        int a = update_height(father,father->firstChild);
    }
    //si le pere a deja un ou plusieurs fils
    else{
        while(tmp_child->nextBro){
           // int b = update_height(tmp_child,tmp_child->nextBro);
            tmp_child = tmp_child->nextBro;
        }
        tmp_child->nextBro = child;
        int c = update_height(tmp_child,tmp_child->nextBro);
    }
}
/=========================== Question 8.4 ======================================/
void print_tree(CellTree* arbre){
    //pour chaque noeud on affiche la hauteur du noeud et la valeur hachee du bloc correspondant
    if(arbre){
        printf("hauteur : %d et val hachee : %02x \n",arbre->height,arbre->block->hash);
    }
    if(arbre->firstChild)
        print_tree(arbre->firstChild);
    if(arbre->nextBro) 
        print_tree(arbre->nextBro);
}

/=========================== Question 8.5 ======================================/
void delete_node(CellTree* node){
    //suppression d'un noeud l'arbre
    if(node == NULL){
        return ;
    }
   else{
      Block * b= node->block; 
       if(b != NULL){
       delete_block(b);
       }
       free(node);
    }
   /* if (node->father) 
        free(node->father);
    if(node->firstChild)
        free(node->firstChild);
    if(node->nextBro)
        free(node->nextBro);

} 

void delete_tree(CellTree** abr){
    // fonction qui supprime l'arbre entiere
     CellTree* tmp ;
     if(*abr == NULL){
       return;
    }
    else {
        tmp = *abr;
         if(tmp->nextBro != NULL)
         delete_tree(&(tmp->nextBro));
        if((tmp)->firstChild != NULL)
        delete_tree(&(tmp->firstChild));
        //if(tmp->firstChild == NULL  && tmp->nextBro == NULL){
           free(tmp);
        //}
    }
}

// -----------------   DETERMINATION DU DERNIER BLOC  ---------------------

/=========================== Question 8.6 ======================================/

CellTree* highest_child(CellTree* cell){
    //fonction qui renvoie le noeud fils avec la plus grande hauteur.
    if(cell == NULL){
        return NULL;
    }
    CellTree * tmp;
    CellTree * high;
    int max = 0;

    //si c'est un seul noeud on le retourne direct
    if( (cell->father == NULL) && (cell->firstChild == NULL) &&  (cell->nextBro == NULL) ){
        max = hauteur(cell);
        return cell;
    }
    if(cell->firstChild != NULL){
         high = cell->firstChild;
         tmp = cell->firstChild->nextBro;
         while(tmp){
            if(high->height < tmp->height){
                high = tmp;
             }
             tmp = tmp->nextBro;
         }
        return high;
    }
    return NULL;
}

/=========================== Question 8.7 ======================================/

CellTree * last_node(CellTree* tree){
    //permettant de retourner la valeur hachee du dernier bloc de cette plus longue chaine.
    
    CellTree * tmp = NULL;
    //if(tree == NULL){
    //    return NULL;
    //}
    tmp = highest_child(tree);
    if(tmp == NULL){
        return tree;;
    }
    else{
        if(tmp->firstChild != NULL || tmp->nextBro != NULL){
            tmp = last_node(tmp); // = highest_child(tmp);
            return tmp;
        }
    }
    return tmp;
}
//-------------------- EXTRACTION DES DE DECLARATIONS DE VOTE -------------------
/=========================== Question 8.8 ======================================/

CellProtected * fusion_listpr(CellProtected* cp1, CellProtected* cp2){
    //fonction realisant la fusion de deux listes chainees de declarations signees. 

    if(cp1 == NULL && cp2 == NULL) return NULL; 
    if(cp1 == NULL) return cp2;
    if(cp2 == NULL) return cp1;

    CellProtected * tmp;
    CellProtected * tmp2;
    Protected * pr;
    //CellProtected * tmp1 = tree1->block->votes;
    while(cp2){
        tmp = cp2;

        pr = tmp->data;
        if(pr == NULL){
            int a = delete_list_protected(&cp1);
            return NULL;
        }
        tmp2 = create_cell_protected(pr);
        if(tmp2 == NULL){
            free(pr->pKey);
            free(pr->mess);
            free(pr->sign->tab) ;
            free(pr->sign);
            free(pr);
            int b2 = delete_list_protected(&cp1);
            return NULL;
        }
        //on ajoute chaque protected de cp2 a cp1
        ajoute_protected_en_tete(&cp1,pr);
        cp2 = cp2->next;
        free(tmp2);
    }
    return cp1;
}


/=========================== Question 8.9 ======================================/
CellProtected * fusion_votehigh(CellTree * tree){
    //retourne la liste obtenue par fusion des listes chainees de declarations contenues dans les blocs de la plus longue chaine.

    if(tree == NULL)
        return NULL;
    
    CellProtected* tmp = NULL;
    printf("test1\n");
    /*tmp = fusion_listpr(tmp,tree->block->votes);
    if(tmp == NULL){
        return NULL;
    }
    CellTree * h = highest_child(tree);
    printf(" high : %d et hash %02x \n",h->height,h->block->hash);
    printf("test2\n");

    if(h){
        printf("test3\n");
        tmp = fusion_listpr(tmp,h->block->votes);
        if(tmp == NULL){
            return NULL;
        }
        printf("mess fusion : %s\n",tmp->data->mess);
        //h = highest_child(h);
    }
    return tmp;
}
*/
