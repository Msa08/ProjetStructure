#include "hachage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


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
	return ((key->n)*(key->val))%size;
}

int find_position(HashTable* t, Key* key){
	/*cherche dans t si key existe*/
	hash_function(key, t->size);
	for(int i=0; i<t->size; i++){
		if(t->tab[i]->key->n == key->n && t->tab[i]->key->val == key->val){
			return i;
		}
	}
	
	return hash_function(key, t->size);
}
		

HashTable* create_hashtable(CellKey* keys, int size){
	HashTable * h = malloc(sizeof(HashTable));
	assert(h);
	h->tab = malloc(sizeof(HashCell*)*size);
	h->size = size;
	
	for(int i=0; i<size; i++){
		h->tab[find_position(h, keys->data)] = create_hashcell(keys->data);
		keys = keys->next;
	}
	
	return h;
}

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
	HashTable hc= create_hashtable(candidates,sizeC);
	HashTable hv= create_hashtable(voters, sizeV);

	while(decl){
		//on verfie si l'electeur a le droit de voter
		if(hv->tab[find_position(decl->data->pkey)]->val==0 && hc->tab[find_position(str_to_key(decl->data->mess))]->val>=0){
			hc->tab[find_position(str_to_key(decl->data->mess))]->val+=1;
			hv->tab[find_position(decl->data->pkey)]->val==1;
		}
		decl=decl->next;
	}

	HashCell gagnant= hc->tab[0];
	for(int i=1; i<sizeC; i++){
		if (hc->tab[i]->val>gagnat->val){
			max=hc->tab[i];
		}
	}
	
	return gaganant->key;
} 
