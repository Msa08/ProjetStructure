#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "exo9.h"

void submit_vote(Protected* p){
    FILE* f = fopen("Pending_votes.txt", "a+");
    // a+: create the file if does not exist and append
    // to the end of the file
    char* str_p = protected_to_str(p);
    fprintf(f,"%s\n", str_p);
    free(str_p);
    fclose(f);
}


void create_block(CellTree* tree, Key* author, int d){
    Block * b = (Block*) malloc(sizeof(Block));
    b->author = author;

    FILE* f = fopen("Pending_votes.txt", "r");
    b->votes = read_protected(f);
    CellTree* last = last_node(tree);

    if (last == NULL){
        b->previous_hash = NULL;
    }
    else {
        b->previous_hash =(unsigned char*) strdup((const char*)last->block->hash);
    }
    //char* s_block = block_to_str(b) ?
    compute_proof_of_work(b, d);
    printf("fin compute\n");

    fclose(f);
    remove("Pending_votes.txt");
    
    FILE* f2 = fopen("Pending_block", "w");
    printf("deb\n");
    ecrireBlock(b, f2);
    printf("fin\n");

    delete_block(b);
    fclose(f2);
}

void add_block(int d, char* name){
    FILE* f = fopen("Pending_block", "r");
    Block* b = lireBlock(f);
    if (verify_block(b, d) == 0){
        delete_block(b);
    }
    else{
        char nom_fichier[256];
        sprintf(nom_fichier,"/Blockchain/%s", name);
        FILE *f2 = fopen(nom_fichier, "w");
        ecrireBlock(b, f2);
        delete_block(b);
        fclose(f2);
    }
    fclose(f);
    remove("Pending_block");
}


CellTree* read_tree(){
    CellTree** T = (CellTree**) malloc(sizeof(CellTree*));
    if (T == NULL){
        return NULL;
    }
    DIR* rep = opendir("/Blockchain/");
    //Etape 1
    int k=1;
    if (rep != NULL){
        struct dirent* dir;
        while ((dir = readdir(rep))){
            FILE* f = fopen(dir->d_name, "r");
            Block* b = lireBlock(f);
            if (k == 1){
                T[k-1] = create_node(b);
            }
            else{
                T = realloc(T, k*sizeof(CellTree*));
                T[k-1] = create_node(b);
            }
            fclose(f);
            k++;
        }
    }
    k--;
    //Etape 2
    for (int i = 0; i<k; k++){
        CellTree* father = T[i];
        for (int j=0; j<k; j++){
            CellTree* child = T[j];
            if (strcmp((const char*)father->block->hash,(const char*)child->block->previous_hash) == 0){
                child->father = father;
                add_child(father,child);
            }
        }
    }

    //Etape 3
    CellTree* racine;
    for (int i = 0; i<k; k++){
        if (T[i]->father == NULL){
            racine = T[i];
            break;
        }
    }
    free(T);
    return racine;
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
   
    CellProtected* declaration = fusion_votehigh(tree);
    filter(&declaration);
    return compute_winner(declaration, candidates, voters, sizeC, sizeV);
}




// AUTRE VERSION
/*
void submit_vote(Protected* p){
    FILE* f = fopen("Pending_votes.txt","a");
    if(f==NULL){
        printf("Erreur à l'ouverture du fichier Pending_votes.txt");
        return;
    }
    char* c=protected_to_str(p);
    fprintf(f,"%s\n",c);

    free(c);
    fclose(f);
    return;
}

void create_block(CellTree* tree, Key* author, int d){

    //Recuperation des votes
    CellProtected* votes = read_protected("Pending_votes.txt");

    CellTree* ln=last_node(tree);

    //Creation du nouveau block
    Block* b = (Block*) malloc(sizeof(Block));

    b->author=author;
    b->votes=votes;
    if(ln!=NULL){
        b->previous_hash=ln->block->hash;
    }
    else{b->previous_hash=NULL;}
    b->nonce=0;

    compute_proof_of_work(b,d);

    //Ajout du block a la fin de l'arbre et update de la taille
    ln->firstChild = create_node(b);
    update_height(ln,ln->firstChild);

    write_block(b,"Pending_block");

    remove("Pending_votes.txt");

}

//Ajoute un block dans le repertoire Blockchain
void add_block(int d, char* name){

    Block* B = read_block("Pending_Block");

    if(verify_block(B,d)){

        char fichier[256]="/Blockchain/";
        strcat(fichier,name);
        FILE* f = fopen(fichier,"w");
        char* bstr=block_to_str(B);
        fprintf(f,"%s\n",bstr);

        free(bstr);
        fclose(f);
    }

    remove("Pending_Block");

}

//Lis tout les fichier de repertoire "Blockchain" et retourne l'arbre construit

CellTree* read_tree(){
    int nbFichiers=0;

    DIR* rep=opendir("./Blockchain/");
    
    if(rep!=NULL){
        struct dirent* dir;

        while((dir = readdir(rep))){

            if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){

                nbFichiers++;

            }

        }

    }

    closedir(rep);

    if(nbFichiers==0){return NULL;}

    rep=opendir("./Blockchain/");
    
    if(rep!=NULL){
        struct dirent* dir;
        int nb_node=0;  //Nombre de noeuds ajouter dans le tableau
        CellTree* racine;

        CellTree* T[nbFichiers];

        while((dir = readdir(rep))){

            if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){

                for(int i = 0;i<nbFichiers;i++){

                    T[i]=create_node(read_block(dir->d_name));
                    nb_node++;

                    for(int j =0; j<nb_node;j++){
                        
                        if(strcmp(T[i]->block->hash,T[j]->block->previous_hash)==0){

                            add_child(T[i],T[j]);

                        }

                    }

                }

            }

        }

        closedir(rep);

        for(int i = 0; i<nbFichiers ; i++){
            if(T[i]->father==NULL){return T[i];}
        }

    }

    closedir(rep);

}
//Fusionne deux listes de declarations
void fusionne_list_decl(CellProtected* decl1, CellProtected* decl2){

    CellProtected* ptr=decl1;

    while(ptr->next!=NULL){
        ptr=ptr->next;
    }

    ptr->next=decl2;

}
//Fusionne les declaration des listes chainee de la plus longue chaine
CellProtected* fusionne_max_chaine(CellTree* Tree){

    CellTree* ptr = highest_child(Tree);
    CellProtected* LCP = ptr->block->votes;

    while(ptr->firstChild!=NULL){
        ptr=highest_child(ptr);
        fusionne_list_decl(LCP,ptr->block->votes);        
    }

    fusionne_list_decl(LCP,ptr->block->votes);
    return LCP;
}

//Determine le gagnant de l'election en se basant sur la plus longue chaine de l'arbre
Key* compute_winner_BT(CellTree* tree, CellKey* candidates,CellKey* voters, int sizeC, int sizeV){

    //extraction des declarations
    CellProtected* LCP = fusionne_max_chaine(tree);

    //suppression des declaration non valide
    filter(LCP);

    //Calcule du vainqueur
    return compute_winner(LCP,candidates,voters, sizeC, sizeV);

}


*/






