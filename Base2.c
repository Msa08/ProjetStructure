#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base2.h"
#include "key.h"
#include "chiffrement.h"

CellKey* create_cell_key(Key* key){
    /* allocation et initialisation d'une cellule de liste chainée de clés */
    CellKey* cell=malloc(sizeof(CellKey));
    cell->data=key;
    cell->next=NULL;
    return cell;
}


CellKey* ajout_en_tete_CellKey(CellKey* cellkey, Key* key){
    if (!cellkey->data)
        return create_cell_key(key);
    
    CellKey* ck = create_cell_key(key);
    ck->next = cellkey;
    return ck;
}

CellKey *read_public_keys(char *nomFic){
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
            ck = ajout_en_tete_CellKey(ck, str_to_key(str));
        }
    }
    return ck;
}


void print_list_keys(CellKey* LCK){
    /* affiche la liste des clés LCK*/
    if(LCK==NULL){
        printf("erreur\n");
        return;
    }
    while(LCK!=NULL){
        printf("%s \t",key_to_str((LCK->data)));
        LCK=LCK->next;
    }
    printf("\n");
}

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

CellProtected* create_cell_protected(Protected* pr){
    /* alloue et initialise une cellule de liste chaînée de déclarations signées */
    CellProtected* cell_p=malloc(sizeof(CellProtected));
    cell_p->data=pr;
    cell_p->next=NULL;
    return cell_p;
}

CellProtected* ajout_en_tete_CellPR(CellProtected *cellpr, Protected *pr){
    if (!cellpr->data)
        return create_cell_protected(pr);

    if(!pr)
        return NULL;
    
    CellProtected *cp = create_cell_protected(pr);
    cp->next = cellpr;
    cellpr=cp;

    return cellpr;
}
CellProtected *read_protected(char *nomFichier){
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL){
        printf("Erreur : ouverture fichier\n");
        return NULL;
    }
    char buffer[256];
    CellProtected *cellpr = create_cell_protected(NULL);
    while(fgets(buffer, 256, f)){
        cellpr = ajout_en_tete_CellPR(cellpr, str_to_protected(buffer));
    }
    fclose(f);
    return cellpr;
}

void print_list_protected(CellProtected* LCP){
    /* affiche la liste de déclarations signées LCP*/
    if(LCP==NULL){
        printf("erreur\n");
        return;
    }
    while(LCP!=NULL){
        printf("%s \n",protected_to_str((LCP->data)));
        LCP=LCP->next;
    }
    printf("\n");
}

void delete_cell_protected(CellProtected* cp){
    /* supprime une cellule de liste chaînée de déclarations signées */
    free(cp->data->pKey);
    free(cp->data->mess);
    free(cp->data->sgn);
    free(cp);
}

void delete_list_protected(CellProtected* LCP){
    /* supprime la liste chaînée de déclarations signées */
    CellProtected* LCP2;
    while(LCP!=NULL){
        LCP2=LCP->next;
        delete_cell_protected(LCP);
        LCP=LCP2;
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






