#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"

CellKey* create_cell_key(Key* key){
    CellKey* cell=malloc(sizeof(CellKey));
    cell->data=key;
    cell->next=NULL;
    return cell;
}


void add_cell_key(CellKey** cellKey, Key* key){
	if (!(*cellKey) || !key){
		printf("[Add_Cell_Key Function] Erreur Key = NULL ou *newCellKey = NULL\n");
		return;
	}
	CellKey* newCellKey = create_cell_key(key);
	newCellKey->next = (*cellKey); 
	(*cellKey) = newCellKey;
	return;
}

CellKey* read_public_keys(char* fichier){
    if(strcmp(fichier,"keys.txt")!=0 && strcmp(fichier,"candidates.txt")!=0){
        printf("mauvais fichier\n");
        return NULL;
    }
    FILE *f=fopen(fichier,"r");
    char ligne[50];
    char poubelle[50];
    char pKey[50];
    CellKey* key;
    Key* pkey2;
    CellKey* liste = malloc(sizeof(CellKey));
    liste->data=NULL;
    liste->next=NULL;
    while(!feof(f)){
        fgets(ligne,50,f);
        sscanf(ligne,"%s %s",pKey,poubelle);
        pkey2=str_to_key(pKey);
        if(liste->data==NULL){
            liste->data=str_to_key(pKey);
        }
        else{
            add_cell_key(&liste, pkey2);
        }
    }
    return liste;
}

void print_list_keys(CellKey* LCK){
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
    free(c->data);
    free(c);
}

void delete_list_keys(CellKey* LCK){
    CellKey* LCK2;
    while(LCK!=NULL){
        LCK2=LCK->next;
        delete_cell_key(LCK);
    }
}