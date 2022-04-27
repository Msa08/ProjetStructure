#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"

//question 5.1
CellKey* create_cell_key(Key* key){
    /* allocation et initialisation d'une cellule de liste chainée de clés */
    CellKey* cell=malloc(sizeof(CellKey));
    cell->data=key;
    cell->next=NULL;
    return cell;
}



//question 5.2
CellKey* add_cell_key(CellKey* cellkey, Key* key){
    /*fonction qui ajoute une clé en tete de liste*/
    if (!cellkey->data)
        return create_cell_key(key);
    
    CellKey* ck = create_cell_key(key);
    ck->next = cellkey;
    return ck;
}



//question 5.3
CellKey *read_public_keys(char *nomFic){
    /*fonction qui retourne une liste chaînée contenant toutes les clés du fichier nomFic*/
    FILE *f = fopen(nomFic, "r");
    if (f == NULL){
        printf("Erreur : ouverture fichier\n");
        return NULL;
    }
    char buffer[100];
    CellKey* ck;
    ck=create_cell_key(NULL);
    char str[100];
    while(fgets(buffer, 100, f)){
        if (sscanf(buffer, " %s\n", str)==1){
            Key* key=str_to_key(str);
            ck = add_cell_key(ck, key);
        }
    }
    fclose(f);
    return ck;
}

//question 5.4
void print_list_keys(CellKey* LCK){
    /* affiche la liste des clés LCK*/
    if(LCK==NULL){
        printf("erreur\n");
        return;
    }
    char* keys;
    while(LCK!=NULL){
        keys=key_to_str((LCK->data));
        printf("%s \t",keys);
        LCK=LCK->next;
        free(keys);
    }
    printf("\n");
}

//question 5.5
void delete_cell_key(CellKey* c){
    /* supprime une cellule de liste chaînée de clés */
    free(c->data);
    free(c);
}

void delete_list_keys(CellKey* LCK){
    /* supprime la liste chaînée de clé c */
    CellKey* LCK2;
    while(LCK!=NULL){
        LCK2=LCK->next;
        delete_cell_key(LCK);
        LCK=LCK2;
    }
}

//question 5.6
CellProtected* create_cell_protected(Protected* pr){
    /* alloue et initialise une cellule de liste chaînée de déclarations signées */
    CellProtected* cell_p=malloc(sizeof(CellProtected));
    cell_p->data=pr;
    cell_p->next=NULL;
    return cell_p;
}


//question 5.7
CellProtected* add_cell_protected(CellProtected *cellpr, Protected *pr){
    /*fonction qui ajoute une déclaration signée en tete de liste*/
    if (!cellpr->data)
        return create_cell_protected(pr);

    if(!pr)
        return NULL;
    
    CellProtected *cp = create_cell_protected(pr);
    cp->next = cellpr;
    cellpr=cp;

    return cellpr;
}

//question 5.8
CellProtected *read_protected(char *nomFichier){
    /*fonction qui lit le fichier déclarations.txt et 
    crée une liste contenant les déclarations signées de ce fichier*/
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL){
        printf("Erreur : ouverture fichier\n");
        return NULL;
    }
    char buffer[256];
    Protected* pr;
    CellProtected *cellpr = create_cell_protected(NULL);
    while(fgets(buffer, 256, f)){
        pr=str_to_protected(buffer);
        cellpr = add_cell_protected(cellpr, pr);
    }
    
    fclose(f);
    return cellpr;
}

//question 5.9
void print_list_protected(CellProtected* LCP){
    /* affiche la liste de déclarations signées LCP*/
    if(LCP==NULL){
        printf("erreur\n");
        return;
    }
    char* keys;
    while(LCP!=NULL){
        keys = protected_to_str((LCP->data));
        printf("%s \n",keys);
        LCP=LCP->next;
        free(keys);
    }
    printf("\n");
}

//question 5.10
void delete_cell_protected(CellProtected* cp){
    /* supprime une cellule de liste chaînée de déclarations signées */
    free(cp->data->pKey);
    free(cp->data->mess);
    free(cp->data->sgn->content);
    free(cp->data->sgn);
    free(cp->data);
    free(cp);
}

void delete_list_protected(CellProtected* c){
    /* supprime la liste chaînée de déclarations signées */
    CellProtected* LCP2;
    CellProtected *temp = c;
    if(!c->data){
        printf("Liste vide\n");
        return;
    }
    while(c){
        printf("bjr\n");
        temp=c->next;
        delete_cell_protected(c);
        c=temp;
    }
}
void deleted_invalid_sign(CellProtected *LCP) {
    /* supprime les déclarations dont la cellule n'est pas valide d'une liste chainée de déclarations signées*/
    CellProtected *tmp = LCP;
    CellProtected *tmp2 = tmp; 
    while (tmp) {
        tmp2 = tmp->next;
        if (verify(tmp->data) == 0) {
            delete_list_protected(LCP);
        }
        tmp = tmp2;
    }
}

void merge_list_protected(CellProtected **list1, CellProtected **list2) {
  // 1ere liste est vide
  if (*list1 == NULL) {
    *list1 = *list2;
    // Destruction 2eme liste sans libérer la mémoire
    *list2 = NULL;
  }
}


