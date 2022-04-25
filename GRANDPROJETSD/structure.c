#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "chiffrement.h"
#include "struct.h"
#include <math.h>
#include <assert.h>


/* == 3.2 == :
initialisation d'une clé
*/

void init_key(Key* key, long val, long n){
    key->val=val;
    key->n=n;
}

/* == 3.3 == :
Uyilise le protocole RSA pour initialisée une clé 
publique et une privée (déja allouée)
*/

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
    //Generation de cle :
    long p =random_prime_number(low_size, up_size, 5000);
    long q =random_prime_number(low_size, up_size, 5000);
    while (p == q){
        q =random_prime_number(2, 7, 5000);
    }
    long n, s, u;
    generate_keys_values(p, q, &n, &s, &u);
    //Pour avoir des cles positives :
    if (u < 0){
        long t=(p-1)*(q-1);
        u=u+t ; //on aura toujours s*u mod t = 1
    }

    pKey->val=s;
    pKey->n=n;

    sKey->val=u;
    sKey->n=n;
}

void print_list_keys(CellKey* LCK){
    CellKey *cour = LCK;

    if (!LCK)
        return;

    while(cour){
        char *tmp = key_to_str(cour->data);
        printf(" %s \n", tmp);
        free(tmp);
        cour=cour->next;
    }
}

/* == 3.6 == :
Alloue et initialise la signature avec un tableau deja
alloue
*/

Signature* init_signature(long* content, int size){
    Signature *sign=(Signature *)malloc(sizeof(Signature));
    assert(sign);
    sign->size=size;
    sign->content=content;

    return sign;
}

/* == 3.10 == :
Allocation et initialisation d'une <Protected>
*/

Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
    Protected *pcd=(Protected *)malloc(sizeof(Protected));
    assert(pcd);
    pcd->pKey = pKey;
    pcd->mess = strdup(mess);
    pcd->sgn = sgn;

    return pcd;
}

CellKey* create_cell_key(Key* key){
    CellKey *ck =(CellKey *)malloc(sizeof(CellKey));

    ck->data = key;
    ck->next = NULL;

    return ck;
}

CellKey* ajout_en_tete_CellKey(CellKey* cellkey, Key* key){
    if (!cellkey->data)
        return create_cell_key(key);
    
    CellKey* ck = create_cell_key(key);
    ck->next = cellkey;
    return ck;
}

void delete_cell_key(CellKey* c){
    if (!c)
        return;

    free(c->data);
    free(c);
}

void delete_list_keys(CellKey* c){
    CellKey *temp = c;

    if(!c)
        return;

    while (c->next){
        temp=c->next;
        delete_cell_key(c);
        c=temp;
    }
    free(c->data);
    free(c);
}

/* == 3.4 == :
key_to_str() retourne une chaîne de caractère representant une clé
str_to_key() retourne une cle representant une chaîne de caractère
*/

char* key_to_str(Key* key){
    if (!key->val || !key->n)
        return "Clé vide";
    
    char *c = malloc(sizeof(char)*20);
    // INSERE LES VALEURS DE LA CLE DANS LA CHAINE : 
    sprintf(c, "(%lx,%lx)", key->val, key->n);
    c[strlen(c)] = '\0';
    //strcat(c, "\0");
    

    return c;
}

Key* str_to_key(char* str){
    Key* key=(Key*)malloc(sizeof(Key));
    assert(key);
    // INSERE LES VALEURS DE LA CHAINE DANS LA CLE (si possible)
    if (sscanf(str, "(%lx,%lx)", &key->val, &key->n) == 2){
        return key;
    }
    return NULL;
}


/* == 3.8 == :
Fonctions fournies qui permettent le passage d'une signature
a sa representation en chaine de caracteres et inversement
*/
char *signature_to_str(Signature *sgn){
    char *result=malloc(10*sgn->size*sizeof(char));
    result[0]= '#';
    int pos = 1;
    char buffer[156];
    for (int i=0; i<sgn->size; i++) {
        sprintf(buffer, "%lx", sgn->content[i]);
        for (int j=0; j <strlen(buffer); j++) {
            result[pos] = buffer[j];
            pos = pos+1;
        }
    result[pos]= '#';
    pos = pos+1;
    }
    result[pos] = '\0';
    result = realloc(result,(pos+1)*sizeof(char));
    return result;
}

Signature *str_to_signature(char * str){
    int len = strlen(str);
    long *content =(long*)malloc(sizeof(long)*len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for (int i=0; i<len; i++) {
        if (str[i] != '#') {
            buffer[pos] = str[i];
            pos = pos+1;
        } else {
            if (pos != 0) {
                buffer[pos] = '\0';
                sscanf(buffer, "%lx",&(content[num]));
                num = num+1;
                pos = 0;
            }
        }
    }
    content = realloc(content, num*sizeof(long));
    return init_signature(content, num);
}

/* == 3.12 == :
Fonctions qui permettent le passage d'un <Protected>
a sa representation en chaine de caracteres et inversement
*/

char *protected_to_str(Protected *pr){

    if(!pr)
        return "vide";

    char *result=malloc(sizeof(char)*100);
    assert(result);

    char* tmp;
    tmp = key_to_str(pr->pKey);
    strcpy(result, tmp);
    strcat(result, " ");
    free(tmp);

    tmp = strdup(pr->mess);
    strcat(result, tmp);
    strcat(result, " ");
    free(tmp);
    
    tmp = signature_to_str(pr->sgn);
    strcat(result, tmp);
    free(tmp);
    
    return result;
}

Protected* str_to_protected(char *str){
    char buffer_cle[100];
    char buffer_msg[256];
    char buffer_sgn[256];

    if (sscanf(str, " %s %s %s\n", buffer_cle, buffer_msg, buffer_sgn) == 3){
        Key *key = str_to_key(buffer_cle);
        Signature * sgn = str_to_signature(buffer_sgn);
        
        return init_protected(key, buffer_msg, sgn);
    }
    return NULL;
}

char* block_to_str(Block* block){
    char* strblock = (char *)malloc(sizeof(char)*1000);
    char* tmp = key_to_str(block->author);
    strcpy(strblock, tmp);
    free(tmp);
    strcat(strblock, " ");
    strcat(strblock, block->previous_hash);
    
    strcat(strblock, " ");
    
    CellProtected *it = block->votes;
    while(it){

        char *pr = protected_to_str(it->data);
        strcat(strblock, pr);
        strcat(strblock, " ");
        free(pr);
        it = it->next;
    }

    char *c = malloc(sizeof(char)*10);
    sprintf(c, "%d\0", block->nonce);
    strcat(strblock, c);
    
    
    return strblock;
}

CellProtected* create_cell_protected(Protected* pr){
    CellProtected *cellpr = (CellProtected *)malloc(sizeof(CellProtected));
    cellpr->data=pr;
    cellpr->next=NULL;
    return cellpr;    
    
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

void afficher_cellPR(CellProtected* cellpr){
    CellProtected *cour = cellpr;
    
    if (!cellpr->data){
        printf("Liste vide\n");
        return;
    }

    while (cour){
        char *tmp = protected_to_str(cour->data);
        printf(" %s\n", tmp);
        free(tmp);
        cour=cour->next;
    }
}

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
    while (c){
        temp=c->next;
        delete_cell_protected(c);
        c=temp;
    }
}

void delete_fraud_list_protected(CellProtected** c){
    CellProtected *cour = *c;
    CellProtected *temp;
    CellProtected *prec;

    while (verify(cour->data)==0){
        char *str = protected_to_str(cour->data);
        free(str);
        temp = cour->next;
        delete_cell_protected(cour);
        cour=temp;
        *c=cour;
    }
        

    while (cour->next){
        if (verify(cour->data)==0){
            char *tmp = protected_to_str(cour->data);
            free(tmp);
            prec->next = cour->next;
            delete_cell_protected(cour);
            cour=prec->next;
        } else {
            char *tmp = protected_to_str(cour->data);
            free(tmp);
            prec=cour;    
            cour=cour->next;
        }
    }

    if (verify(cour->data) == 0){
        char *tmp = protected_to_str(cour->data);
        free(tmp);
        delete_cell_protected(cour);
    } else {
        char *tmp = protected_to_str(cour->data);
        free(tmp);
    }

}

HashCell* create_hashcell(Key* key){
    HashCell *h = (HashCell*)malloc(sizeof(HashCell));
    h->key = key;
    h->val = 0;

    return h;
}

int find_position(HashTable* t, Key* key){

    for(int i=0; i<t->size; i++){ //parcours du tableau
        if( ((t->tab[i])->key)->val == key->val && ((t->tab[i])->key)->n == key->n ) //test clé
            return i;
    }
    //printf("Pas de clé\n");
    return hash_function(key, t->size); //retourne la position où il aurait dû être
}

HashTable* create_hashtable(CellKey* keys, int size){
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

void delete_hashtable(HashTable* t){

    for(int i; i<t->size; i++){
        free(t->tab[i]->key);       
    }

    free(t->tab);
    free(t);
}

void delete_block(Block* b){ 

    free(b->hash);
    free(b->previous_hash);
    CellProtected *tmp = b->votes;
    while(b->votes){
        tmp=b->votes->next;
        free(b->votes);
        b->votes=tmp;
    }
}


CellTree* create_node(Block* b){
    CellTree *node = (CellTree *)malloc(sizeof(CellTree));
    node->block = b;
    node->father = NULL;
    node->firstChild = NULL;
    node->nextBro = NULL;
    node->height = 0;

    return node;
}

int update_height(CellTree* father, CellTree* child){
    if (child->height >= father->height){
        father->height = child->height+1;
        return 1;
    }
    return 0;
}

void add_child(CellTree* father, CellTree* child){

    if(!father){ // test si arbre vide, si oui le crée
        father = create_node(child->block);
        printf("daron pas la\n");
        return;
    }

    CellTree *debut=father; 
    if (!father->firstChild){ // test si le noeud n'a pas de fils
        father->firstChild = child;
        child->block->previous_hash = father->block->hash;
        child->father=father;
        
        CellTree *it = father;
        CellTree *itsuiv = child;

        while(it){// update de la taille des noeuds
            update_height(it, itsuiv);
            it=it->father;
            itsuiv=itsuiv->father;
        }
        father=debut;
        return;
    }

    CellTree *it = father->firstChild;
    while(it->nextBro){
            it=it->nextBro;
    }
    it->nextBro = child;
    child->block->previous_hash = father->block->hash;
    child->father=father;
    
    CellTree *itr = father;
    CellTree *itrsuiv = child;
    while(itr){
        update_height(father, child);
        itr=itr->father;
        itrsuiv=itrsuiv->father;
    }

    father=debut;
}

void print_tree(CellTree* node){
    if(!node || !node->block){
        printf("Arbre vide\n");
        return;
    }
    printf("hauteur : %d\n", node->height);
    printf("hash : %s\n", node->block->hash);
    printf("previous_hash : %s\n", node->block->previous_hash);
    printf("\n");

    CellTree *cour=node->firstChild;
    while(cour){
        
        print_tree(cour);
        cour=cour->nextBro;
    }   
}

void delete_node(CellTree* node){
    delete_block(node->block);

    if(node->father)
        free(node->father);
    if(node->firstChild)
        free(node->firstChild);
    if(node->nextBro)
        free(node->nextBro);
    
    free(node);
}

void delete_tree(CellTree* node){
    if(!node->block){
        printf("Arbre vide\n");
        return;
    }

    CellTree *cour=node->firstChild;
    while(cour){
        if(!cour->nextBro){
            delete_node(cour); 
            return;
        }
            
        delete_tree(cour);
        cour=cour->nextBro;
    }
}


CellTree* highest_child(CellTree* cell){
    int max = 0;
    CellTree *hchild = malloc(sizeof(CellTree));
    if(max <= cell->firstChild->height){
        max = cell->firstChild->height;
        hchild = cell->firstChild;

    }
    else{
        CellTree *node = cell->firstChild->nextBro;
        while(node){
            if(max <= node->height){
                max = node->height;
                hchild = node;
            }
            node = node->nextBro;
        }
    }

    return hchild;
}

CellTree* last_node(CellTree* tree){
    if(!tree)
        return NULL;
    if(!tree->block){
        //printf("block vide\n");
        return NULL;
    }
    CellTree *tmp = (CellTree *)malloc(sizeof(CellTree));
    tmp = highest_child(tree);
    while(tmp->height>0){
        tmp = highest_child(tmp);
    }

    return tmp;
}

CellProtected *fusion(CellProtected *liste1, CellProtected* liste2){
    if(!liste1)
        return liste2;
    if(!liste2){
        return liste1;
    }

    CellProtected *debut = liste1;
    while(liste1->next){
        liste1=liste1->next;
    }

    liste1->next = liste2;
    liste1=debut;
    return liste1;

    //8.8) Pour cela il faudrait rajouter un champs 
    // dernier_element dans la structure CellProtected

}

CellProtected *fusion_decl(CellTree *tree){
    CellProtected *res = (CellProtected *)malloc(sizeof(CellProtected));
    res=tree->block->votes;
    CellTree *tmp = highest_child(tree);
    while(tmp->height > 0){
        res=fusion(res, tmp->block->votes);
        tmp=highest_child(tmp);
    } 
    res=fusion(res, tmp->block->votes);
    return res;
}

int verify_block(Block* b, int d){
    // retourne 1 si le block est valide, 0 sinon
    char* block;
    unsigned char *tmp;
    block = block_to_str(b);
    tmp = str_to_hash(block);
 
    for (int i = 0; i<d; i++){
        if(tmp[i] != '0'){
            return 0;
        }
    }
    return 1;
}

/* == 3.11 == :
Verifie que la signature dans pr correspond au message
et a la personne dans pr
*/
int verify(Protected* pr){
    // decryptage du contenu dans la signature 
    char *tmp = decrypt(pr->sgn->content, pr->sgn->size, pr->pKey->val, pr->pKey->n);

    // comparaison avec le message dans pr 
    if (strcmp(tmp, pr->mess))
        return 0;
    
    free(tmp);
    return 1;
}