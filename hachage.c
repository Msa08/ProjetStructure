#include "hachage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "key.h"

//question 6.1
void filter(CellProtected** LCP){
    /* Supprime les declarations dont la signature n'est
    pas valide.
    */
    CellProtected* courant = *LCP;

    // Filtre les premiers jusqu'a une signature valide
    while (*LCP != NULL && verify((*LCP)->data) == 0){

        courant = *LCP;
        if ((*LCP)->next == NULL){
            printf("Aucun\n");
            delete_cell_protected(*LCP);
            *LCP = NULL;
            printf("Aucune declarations signees valides\n");
            return ;
        }
        *LCP = (*LCP)->next;
        delete_cell_protected(courant);
    }


    // Filtre les prochains
    if(*LCP == NULL){
        *LCP = NULL;
        printf("Aucune declarations signees valides\n");
        return ;
    }
    CellProtected* precedent = *LCP;
    courant = precedent->next;
    while(courant != NULL){
        if (courant->data != NULL && 
            verify(courant->data) == 0){
            precedent->next = courant->next;
            delete_cell_protected(courant);
        }else{
            precedent = courant;
        }
        courant = precedent->next;
    }
}

//question 6.2
HashCell* create_hashcell(Key* key){
	/*fonction permettant d'allouer et initialiser à 0 une cellule de la table de hachage*/
	HashCell* h = malloc(sizeof(HashCell));
	h->key = key;
	h->val = 0;

	return h;
}


void delete_hashcell(HashCell* c){
	free(c);
}

//question 6.3
int hash_function(Key* key, int size){
	/*retourne la position d'un élément dans la table de hachage,
	si la table est vide retourne 0*/
	if (key != NULL) {
		return (key->cle + key->n) % size;
	}
	return 0;
}

// question 6.4
int find_position(HashTable* t, Key* key){
	/*cherche dans la table t si l'élément key existe et retourne sa position*/
	int h, i=0;
	while(i < t->size){
		h=(hash_function(key,t->size)+i)%t->size;
		if(t->tab[h]!=NULL && (t->tab[h]->key->cle == key->cle) && (t->tab[h]->key->n == key->n)){
			return h;
		}
		i++;
	}
	return i;
}

//question 6.5
HashTable* create_hashtable(CellKey* keys, int size){
	/*fonction permettant de créer et initialiser une table de hachage*/
    HashTable *t = (HashTable*)malloc(sizeof(HashTable));
    t->tab = (HashCell**)malloc(sizeof(HashCell*)*size);
    t->size=size;
    
    for(int i=0; i<t->size; i++){
        t->tab[i]=create_hashcell(NULL);
    }
    int i = 0;
    while(keys && i<size){
        int pos = hash_function(keys->data, size);
      
        if( !t->tab[pos]->key ){
            t->tab[pos]->key = keys->data;
        
        }else{
            while(pos>=0){
                if(pos == size)
                    pos=0;

                if( !t->tab[pos]->key){  
                    t->tab[pos]->key = keys->data;
                    break;
                }
                pos++;
            }   
        }
        i++;
        keys=keys->next;
    }
    return t;
}

//question 6.6
void delete_hashtable(HashTable* t){
	/*fonction permettant de supprimer une table de hachage t*/
    for(int i=0;i<t->size;i++){
        free(t->tab[i]);
    }
	free(t->tab);
	free(t);
}

//question 6.7
Key* compute_winner(CellProtected* decl, CellKey* candidates,CellKey* voters, int sizeC, int sizeV){
	/*fonction permettant de calculer le vainqueur d'une élection*/
	//Creation de 2 tables de hachages
	HashTable* hc= create_hashtable(candidates,sizeC);//pour la liste des candidats
	HashTable* hv= create_hashtable(voters, sizeV);//pour la liste des votants
	
	if(decl==NULL){
		return NULL;
	}

	//COMPTAGE DU NOMBRE DE VOTE POUR CHAQUE CANDIDAT
	int positionV, positionC;
	while(decl != NULL){//on parcourt la liste de declarations
		positionV = find_position(hv, decl->data->pKey);//on recupere la position dans hv de l'emetteur de la declaration

		Key * Ckey = str_to_key(decl->data->mess);//on recupere la clé publique du candidat pour qui l'emetteur a voté
		positionC = find_position(hc, Ckey);//on recupere la postion du candidat dans hc
		free(Ckey);
		
		//si le votant ou le candidat n'est pas dans leur liste alors on ne compte pas cette déclaration (le cas ou position = -1)
		if(positionV >= 0 && positionC >= 0 && hv->tab[positionV]->val == 0){//on verifie que le votant n'a pas deja voté
			hv->tab[positionV]->val = 1;// le votant a voté
			(hc->tab[positionC]->val)++;//on increment le nb de vote pour le candidat
		}
		decl = decl->next;
	}

	//ON DETERMINE LE VAINQUEUR
	HashCell* gagnant= hc->tab[0];//au debut gagnant = le premier candidat
	for(int i=1; i<sizeC; i++){//on parcourt le tableau de candidat
		if (hc->tab[i]->val > gagnant->val){//si le candidat courant a plus de vote que gagnant
			gagnant=hc->tab[i];//il devient le gagnant
		}
	}
	char* keyg=key_to_str(gagnant->key);
	delete_hashtable(hc);
	delete_hashtable(hv);
	Key* key_gagnant=str_to_key(keyg);
	free(keyg);
	return key_gagnant;
}

void push_protected (CellProtected ** cp, Protected * pr){
  CellProtected * new_cell = create_cell_protected(pr);
  new_cell->next = *cp;
  *cp = new_cell;
}

CellProtected* cellProtectedDup(CellProtected * cp) {
  if (cp != NULL) {
    CellProtected * res = create_cell_protected(cp->data);
    cp = cp->next;
    while (cp != NULL) {
      push_protected(&res, cp->data);
      cp = cp->next;
    }
    return res;
  }

  return NULL;
}

