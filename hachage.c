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
	printf("entry hash_function\n");
	if (key != NULL) {
			printf("return hash_function\n");
		return (key->cle + key->n) % size;
     	      //return ((key->n)*(key->cle))%size;
	}
	printf("return hash_function2\n");
	return 0;
}
/*int hash_function(Key* key, int size){
printf("entry hash_function\n");
    char *cle = key_to_str(key);
    float A=(sqrt(5)-1)/2.0;

    int somme = 0;// pour additionner le nombre représentant chaque lettre de la cle
    int i = 0;//index de tableau
    while(cle[i]!='\0'){ //chaque chaîne de caractère finit par '\0'
        somme+=(int)(cle[i]);//cast en int pour avoir le nombre et non la lettre
        i++;
    }

    free(cle);
			printf("return hash_function\n");
    return (int)(size*((somme*A)-((int)(somme*A))));

}*/



int find_position(HashTable* t, Key* key){
	//cherche dans t si key existe

	for(int i=0; i<t->size; i++){//parcourt la table de hachage
		//si clé de la ième case de tab = la clé en argument alors return i
		if(t->tab[i]->key->n == key->n && t->tab[i]->key->cle == key->cle){
			printf("return find pos\n");
			return i;
		}
	}
	//sinon retourne la position ou elle aurait du etre
	printf("return1 findpos\n");
	return hash_function(key, t->size);
}

/*int find_position(HashTable* t, Key* key){
 printf("entry find_position\n");
	int h, i=0;
	while(i != t->size){
		h=(hash_function(key,t->size)+i)%t->size;
		printf("h= %d\n",h);
		if(t->tab[h]==NULL || (t->tab[h]->key->cle == key->cle && t->tab[h]->key->n == key->n)){
			printf("return find_position\n");
			return h;
		}
		i++;
	}
	printf("return find_position2\n");
	return i;
}*/
/*int find_position(HashTable* t, Key* key){
	printf("entry find_position\n");
    for(int i=0; i<t->size; i++){ //parcours du tableau
        if( ((t->tab[i])->key)->cle == key->cle && ((t->tab[i])->key)->n == key->n ) //test clé
					printf("return find_position\n");
            return i;
    }
    //printf("Pas de clé\n");
		printf("return find_position\n");
    return hash_function(key, t->size); //retourne la position où il aurait dû être
}*/


/*HashTable* create_hashtable(CellKey* keys, int size){
	printf("entry create_hashtable\n");
	HashTable * h = malloc(sizeof(HashTable));
	//assert(h);
	h->tab = malloc(sizeof(HashCell*)*size);
	h->size = size;

	for(int i=0; i<size; i++){
		h->tab[find_position(h, keys->data)] = create_hashcell(keys->data);
		keys = keys->next;
	}
	printf("return create_hashtable\n");
	return h;
}*/


HashTable* create_hashtable(CellKey* keys, int size){
	printf("entry create_hashtable\n");
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
	printf("return create_hashtable\n");
    return t;
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
	printf("entry in compute_winner\n");
	HashTable* hc= create_hashtable(candidates,sizeC);//pour la liste des candidats
	printf("table de hachage des candidats cree\n");
	HashTable* hv= create_hashtable(voters, sizeV);//pour la liste des votants
	printf("table de hachage des votants cree\n");
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
	//delete_hashtable_
	//(free à ajouter ?)
	printf("return computewinner\n");
	return gagnant->key;
}

/*Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
	printf("entry in compute_winner\n");
	HashTable* Hc = create_hashtable(candidates, sizeC);
	HashTable* Hv = create_hashtable(voters, sizeV);
	printf("table de hachage des votants cree\n");
	int positionV, positionC;
	// comptage des votes
	while(decl != NULL){
		positionV = find_position(Hv, decl->data->pKey);

		Key * Ckey = str_to_key(decl->data->mess);
		positionC = find_position(Hc, Ckey);
		printf("positionC = %d \n", positionC);
		free(Ckey);
		
		//si le votant ou le candidat n'est pas dans leur liste alors on ne compte pas cette déclaration (le cas ou position = -1)
		if(positionV >= 0 && positionC >= 0 && Hv->tab[positionV]->val == 0){
			Hv->tab[positionV]->val = 1;
			(Hc->tab[positionC]->val)++;
		}
		decl = decl->next;
	}
	
	// détermine le gagnant en parcourant Hc
	int position_winner = 0;
	int cpt_winner = 0;
	
	for(int i=0; i<sizeC; i++){
		if(Hc->tab[i]->val > cpt_winner){
			position_winner = i;
			cpt_winner = Hc->tab[i]->val;
		}
		printf("tab[%d] val = %d \n", i, Hc->tab[i]->val);
	}
	
	Key * winner = malloc(sizeof(Key));
	init_key(winner, Hc->tab[position_winner]->key->cle, Hc->tab[position_winner]->key->n);
	
	delete_hashtable(Hc);
	delete_hashtable(Hv);
	printf("return computewinner\n");
	return winner;
	
}*/


//gcc -Wall  -o main base.c chiffrement.c hachage.c key.c prime_number.c
