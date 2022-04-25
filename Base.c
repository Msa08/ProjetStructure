<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"

CellKey* create_cell_key(Key* key){
    /* allocation et initialisation d'une cellule de liste chainée de clés */
    CellKey* cell=malloc(sizeof(CellKey));
    cell->data=key;
    cell->next=NULL;
    return cell;
}


// void add_cell_key(CellKey** cellKey, Key* key){
// 	/*ajout d'une clé en tete de liste*/
// 	if (!(*cellKey) || !key){
// 		printf("[Add_Cell_Key Function] Erreur Key = NULL ou *newCellKey = NULL\n");
// 		return;
// 	}
// 	CellKey* newCellKey = create_cell_key(key);
// 	newCellKey->next = (*cellKey); 
// 	(*cellKey) = newCellKey;
// 	return;
// }
CellKey* add_cell_key(CellKey* cellkey, Key* key){
    if (!cellkey->data)
        return create_cell_key(key);
    
    CellKey* ck = create_cell_key(key);
    ck->next = cellkey;
    return ck;
}

// CellKey* read_public_keys(char* fichier){
//     /*retourne une liste chainée contenant toutes les clés du fichier en paramètre*/
//     if(strcmp(fichier,"keys.txt")!=0 && strcmp(fichier,"candidates.txt")!=0){
//         printf("mauvais fichier\n");
//         return NULL;
//     }
//     FILE *f=fopen(fichier,"r");
//     char ligne[50];
//     char poubelle[50];
//     char pKey[50];
//     Key* pkey2;
//     CellKey* liste = malloc(sizeof(CellKey));
//     liste->data=NULL;
//     liste->next=NULL;
//     while(!feof(f)){
//         fgets(ligne,50,f);
//         sscanf(ligne,"%s %s",pKey,poubelle);
//         pkey2=str_to_key(pKey);
//         if(liste->data==NULL){
//             liste->data=str_to_key(pKey);
//         }
//         else{
//             add_cell_key(&liste, pkey2);
//         }
//     }
//     return liste;
// }
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
            ck = add_cell_key(ck, str_to_key(str));
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

// void add_cell_protected(CellProtected** cellPro, Protected* pr){
// 	/* ajoute une déclaration signée en tête de liste*/
// 	if (!(*cellPro) || !pr){
// 		printf("[Add_Cell_Key Function] Erreur Key = NULL ou *newCellKey = NULL\n");
// 		return;
// 	}
// 	CellProtected* newCellProtected = create_cell_protected(pr);
// 	newCellProtected->next = (*cellPro); 
// 	(*cellPro) = newCellProtected;
// 	return;
// }

CellProtected* add_cell_protected(CellProtected *cellpr, Protected *pr){
    if (!cellpr->data)
        return create_cell_protected(pr);

    if(!pr)
        return NULL;
    
    CellProtected *cp = create_cell_protected(pr);
    cp->next = cellpr;
    cellpr=cp;

    return cellpr;
}

// CellProtected* read_protected(char* fic){
//     /* retourne une liste chaînée contenant toutes les déclarations signées du fichier
//     “declarations.txt” */
//     if(strcmp(fic,"declarations.txt")!=0){
//         printf("mauvais fichier\n");
//         return NULL;
//     }
//     FILE *f = fopen(fic,"r");
//     CellProtected* liste=malloc(sizeof(CellProtected));
//     liste->data=NULL;
//     liste->next=NULL;
//     char ligne[50];
//     char poubelle[50];
//     char protected[50];
//     Protected* pr2;
//     while(!feof(f)){
//         fgets(ligne,50,f);
//         sscanf(ligne,"%s %s",protected,poubelle);
//         pr2=str_to_protected(protected);
//         if(liste->data==NULL){
//             liste->data=str_to_protected(protected);
//         }
//         else{
//             add_cell_protected(&liste, pr2);
//         }
//     }
//     return liste;
// }
CellProtected *read_protected(char *nomFichier){
    FILE *f = fopen(nomFichier, "r");
    if (f == NULL){
        printf("Erreur : ouverture fichier\n");
        return NULL;
    }
    char buffer[256];
    CellProtected *cellpr = create_cell_protected(NULL);
    while(fgets(buffer, 256, f)){
        cellpr = add_cell_protected(cellpr, str_to_protected(buffer));
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

=======
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"

CellKey* create_cell_key(Key* key){
    /* allocation et initialisation d'une cellule de liste chainée de clés */
    CellKey* cell=malloc(sizeof(CellKey));
    cell->data=key;
    cell->next=NULL;
    return cell;
}


// void add_cell_key(CellKey** cellKey, Key* key){
// 	/*ajout d'une clé en tete de liste*/
// 	if (!(*cellKey) || !key){
// 		printf("[Add_Cell_Key Function] Erreur Key = NULL ou *newCellKey = NULL\n");
// 		return;
// 	}
// 	CellKey* newCellKey = create_cell_key(key);
// 	newCellKey->next = (*cellKey); 
// 	(*cellKey) = newCellKey;
// 	return;
// }
CellKey* add_cell_key(CellKey* cellkey, Key* key){
    if (!cellkey->data)
        return create_cell_key(key);
    
    CellKey* ck = create_cell_key(key);
    ck->next = cellkey;
    return ck;
}

// CellKey* read_public_keys(char* fichier){
//     /*retourne une liste chainée contenant toutes les clés du fichier en paramètre*/
//     if(strcmp(fichier,"keys.txt")!=0 && strcmp(fichier,"candidates.txt")!=0){
//         printf("mauvais fichier\n");
//         return NULL;
//     }
//     FILE *f=fopen(fichier,"r");
//     char ligne[50];
//     char poubelle[50];
//     char pKey[50];
//     Key* pkey2;
//     CellKey* liste = malloc(sizeof(CellKey));
//     liste->data=NULL;
//     liste->next=NULL;
//     while(!feof(f)){
//         fgets(ligne,50,f);
//         sscanf(ligne,"%s %s",pKey,poubelle);
//         pkey2=str_to_key(pKey);
//         if(liste->data==NULL){
//             liste->data=str_to_key(pKey);
//         }
//         else{
//             add_cell_key(&liste, pkey2);
//         }
//     }
//     return liste;
// }
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
    Key* key;
    while(fgets(buffer, 100, f)){
        if (sscanf(buffer, " %s\n", str)==1){
            key=str_to_key(str);
            ck = add_cell_key(ck, str_to_key(str));
            free(key);
        }
    }
    fclose(f);
    return ck;
}


void print_list_keys(CellKey* LCK){
    /* affiche la liste des clés LCK*/
    char *buffer;
    if(LCK==NULL){
        printf("erreur\n");
        return;
    }
    while(LCK!=NULL){
        buffer=key_to_str((LCK->data));
        printf("%s \t",buffer);
        LCK=LCK->next;
        free(buffer);
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

CellProtected* add_cell_protected(CellProtected *cellpr, Protected *pr){
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
        Protected* pr=str_to_protected(buffer);
        cellpr = add_cell_protected(cellpr, pr);
    }
    fclose(f);
    return cellpr;
}

void print_list_protected(CellProtected* LCP){
    /* affiche la liste de déclarations signées LCP*/
    char *buffer;
    if(LCP==NULL){
        printf("erreur\n");
        return;
    }
    while(LCP!=NULL){
        buffer=protected_to_str((LCP->data));
        printf("%s \n",buffer);
        LCP=LCP->next;
        free(buffer);
    }
    printf("\n");
}

/*void delete_cell_protected(CellProtected* cp){
    /* supprime une cellule de liste chaînée de déclarations signées 
    free(cp->data->pKey);
    free(cp->data->mess);
    free(cp->data->sgn);
    free(cp);
}

void delete_list_protected(CellProtected* LCP){
    /* supprime la liste chaînée de déclarations signées 
    CellProtected* LCP2;
    while(LCP!=NULL){
        LCP2=LCP->next;
        delete_cell_protected(LCP);
        LCP=LCP2;
    }
}*/

void delete_cell_protected(CellProtected* c){
    if (!c)
        return;

    free(c->data->pKey);
    free(c->data->mess);
    free(c->data->sgn->content);
    free(c->data->sgn);
    free(c->data);
    free(c);
}

void delete_list_protected(CellProtected* c){
    CellProtected *temp = c;
    if(!c->data){
        printf("Liste vide\n");
        return;
    }
    while(c){
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

>>>>>>> 69701f70dca74916819c95df83e9ad54ccea4d33
