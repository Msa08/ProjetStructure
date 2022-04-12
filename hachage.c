#include "hachage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>


HashCell* create_hashcell(Key* key){
	HashCell* h = malloc(sizeof(HashCell));
	h->key = key;
	h->val = 0;
	
	return h;
}

void delete_hashcell(HashCell* c){
	free(c);

}

int hash_function(Key* key, int size){
	return ((key->n)*(key->cle))%size;
}

int find_position(HashTable* t, Key* key){
	/*cherche dans t si key existe*/
	hash_function(key, t->size);
	for(int i=0; i<t->size; i++){//parcourt la table de hachage
		//si clé de la ième case de tab = la clé en argument alors return i
		if(t->tab[i]->key->n == key->n && t->tab[i]->key->cle == key->cle){
			return i;
		}
	}
	//sinon retourne la position ou elle aurait du etre 
	return hash_function(key, t->size);
}
		

HashTable* create_hashtable(CellKey* keys, int size){
	HashTable * h = malloc(sizeof(HashTable));
	//assert(h);
	h->tab = malloc(sizeof(HashCell*)*size);
	h->size = size;
	
	for(int i=0; i<size; i++){
		h->tab[find_position(h, keys->data)] = create_hashcell(keys->data);
		keys = keys->next;
	}
	
	return h;
}


	
void delete_hashtable(HashTable* t){
	for(int i=0 ; i < t->size; i++){
		free(t->tab[i]->key);
		free(t->tab[i]);
	}
	free(t->tab);
	free(t);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates,CellKey* voters, int sizeC, int sizeV){
	//Creation de 2 tables de hachages
	HashTable* hc= create_hashtable(candidates,sizeC);//pour la liste des candidats
	HashTable* hv= create_hashtable(voters, sizeV);//pour la liste des candidats

	if(decl==NULL){
		return NULL;
	}

	while(decl){//on parcourt la liste des déclarations

		//Test si le votant a le droit de voté (il est dans hv?)
		if(hv->tab[find_position(hv,(decl->data->pKey))]->key==decl->data->pKey){
			//Test si le votant n'a pas deja voté(val=0)
			if(hv->tab[find_position(hv,(decl->data->pKey))]->val==0 ){
				//Test si le candidat est bien un candidat de l'election
				if(hc->tab[find_position(hc,(str_to_key(decl->data->mess)))]->key==(str_to_key(decl->data->mess))){
					hc->tab[find_position(hc,(str_to_key(decl->data->mess)))]->val+=1;//+1 vote
					hv->tab[find_position(hv,(decl->data->pKey))]->val=1;// a deja voté
				}
			}
		}
		decl=decl->next;
	}
	
	//On determine le vainqueur
	HashCell* gagnant= hc->tab[0];//au debut gagnant = le premier candidat
	for(int i=1; i<sizeC; i++){//on parcourt le tableau de candidat
		if (hc->tab[i]->val > gagnant->val){//si le candidat courant a plus de vote que gagnant 
			gagnant=hc->tab[i];//il devient le gagnant
		}
	}
	
	//(free à ajouter ?)
	return gagnant->key;
} 


//gcc -Wall  -o main base.c chiffrement.c hachage.c key.c prime_number.c