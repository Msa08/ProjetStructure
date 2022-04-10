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

int hash_function(Key* key, int size){
	return ((key->n)*(key->cle))%size;
}

