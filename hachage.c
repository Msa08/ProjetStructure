#include "hachage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include "key.h"


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
	//printf("entry hash_function\n");
	if (key != NULL) {
			//printf("return hash_function\n");
		return (key->cle + key->n) % size;
     	      //return ((key->n)*(key->cle))%size;
	}
	//printf("return hash_function2\n");
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


//influe pas
/*int find_position(HashTable* t, Key* key){
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
}*/

int find_position(HashTable* t, Key* key){
 //printf("entry find_position\n");
	int h, i=0;
	while(i < t->size){
		h=(hash_function(key,t->size)+i)%t->size;
		//printf("h= %d\n",h);
		//printf("%ld, %ld\n",t->tab[h]->key->cle, t->tab[h]->key->n);
		if(t->tab[h]!=NULL && (t->tab[h]->key->cle == key->cle) && (t->tab[h]->key->n == key->n)){
			//printf("return find_position\n");
			return h;
		}
		i++;
	}
	//printf("return find_position2\n");
	return i;
}

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

//fonctionne pas
/*HashTable* create_hashtable(CellKey* keys, int size){
	printf("entry create_hashtable\n");
	HashTable* h =(HashTable*) malloc(sizeof(HashTable));
	//assert(h);
	h->tab = (HashCell**)malloc(sizeof(HashCell*)*size);
	h->size = size;

	  for(int i=0; i<h->size; i++){
        h->tab[i]=create_hashcell(NULL);
    }

	for(int i=0; i<size; i++){
		h->tab[find_position(h, keys->data)] = create_hashcell(keys->data);
		keys = keys->next;
	}
	printf("return create_hashtable\n");
	return h;
}*/


HashTable* create_hashtable(CellKey* keys, int size){
	//printf("entry create_hashtable\n");
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
	//printf("return create_hashtable\n");
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
		printf("tab[%s] val = %d \n",key_to_str( hc->tab[i]->key), hc->tab[i]->val);
	}
	


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
		//printf("positionC = %d \n", positionC);
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
