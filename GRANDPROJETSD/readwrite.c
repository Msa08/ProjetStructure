#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "chiffrement.h"
#include "readwrite.h"
#include "compute.h"
#include "struct.h"
#include <math.h>
#include <assert.h>
#include <dirent.h>


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
            ck = ajout_en_tete_CellKey(ck, str_to_key(str));
        }
    }
    return ck;
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
        cellpr = ajout_en_tete_CellPR(cellpr, str_to_protected(buffer));
    }
    fclose(f);
    return cellpr;
}

void print_file_block(Block *block, char *name){
    FILE *f = fopen(name, "w");
    char *auth = key_to_str(block->author);
    fprintf(f, "%s\n", auth);
    free(auth);
    CellProtected *tmp = block->votes;
    while(tmp){
        char *pr = protected_to_str(tmp->data);
        fprintf(f, "%s\n", pr);
        free(pr);
        tmp = tmp->next;
    }
    fprintf(f, "%s\n", block->hash);
    fprintf(f, "%s\n", block->previous_hash);
    fprintf(f, "%d\n", block->nonce);
    fclose(f);
}

Block *read_file_block(char *nf){
    FILE *f = fopen(nf, "r");
    Block *block = (Block *)malloc(sizeof(Block));
    char buffer[256];
    block->votes = create_cell_protected(NULL);
    CellProtected *droit = create_cell_protected(NULL);
    int nonce;

    if(fgets(buffer, 256, f)){
        block->author=str_to_key(buffer);
    }


    while(fgets(buffer, 256, f)){ 
        char *tmp=malloc(sizeof(char)*256);
        char *tmp1=malloc(sizeof(char)*120);
        char *tmp2=malloc(sizeof(char)*50);
        char *tmp3=malloc(sizeof(char)*100);
        
        if(sscanf(buffer, " %s %s %s\n", tmp1, tmp2, tmp3) == 3){
            strcpy(tmp, tmp1);
            strcat(tmp, " ");
            strcat(tmp, tmp2);
            strcat(tmp, " ");
            strcat(tmp, tmp3);
            strcat(tmp, "\n");
            
            Protected *pr = str_to_protected(tmp);
            block->votes = ajout_en_tete_CellPR(block->votes, pr);

        }
        else{
            free(tmp);
            block->hash=strdup(tmp1);
            free(tmp2);
            free(tmp3);
            break;
        }

        free(tmp);
        free(tmp1);
        free(tmp2);
        free(tmp3);
    }

    if(fgets(buffer, 256, f)){
        buffer[strlen(buffer)-1]='\0';
        block->previous_hash=strdup(buffer);
    }
    
    if(fgets(buffer, 256, f)){    
        if(sscanf(buffer, "%d", &nonce) == 1){
            block->nonce=nonce;
        }
    }

    fclose(f);

    while(block->votes){
        droit=ajout_en_tete_CellPR(droit, block->votes->data);
        block->votes=block->votes->next;
    }
    delete_cell_protected(block->votes);
    block->votes=droit;
    return block;
}

void submit_vote(Protected* p){
    FILE *f=fopen("Pending_votes.txt","a");
    char *str = protected_to_str(p);
    fprintf(f,"%s\n", str);
    free(str);
    fclose(f);
}

void create_block(CellTree* tree, Key* author, int d){
    //lecture du fichier afin de recuperer la declaration signée
    CellProtected *pr = read_protected("Pending_votes.txt"); 
    // création du block
    Block *b = (Block *)malloc(sizeof(Block));
    b->author=author;
    b->votes=pr;
    b->nonce=0;
    // reconstitution de la chaine de hash
    if(!tree->block){
        b->previous_hash=strdup(" ");
        
    }else{
        b->previous_hash=strdup(tree->block->hash);
    }
    
    char *block = block_to_str(b);
    b->hash=str_to_hash(block);
    free(block);
    compute_proof_of_work(b, d);

    //sauvegarde du dernier block crée
    tree->block = b;
    
    // création du fichier Pending_block afin de stocker le block et supprimer le fichier des votes en attente 
    print_file_block(b, "Pending_block.txt");
    remove("Pending_votes.txt");

}

void add_block(int d, char* name){
    Block *b = read_file_block("Pending_block.txt"); //lecture du fichier afin de recuperer le block
    
    if (verify_block(b, d) == 1){ // verifie si le block est valide, si oui, cree un fichier "name"
        chdir("Blockchain/");
        print_file_block(b, name);
    }
    chdir("../");
    remove("Pending_block.txt"); // suppression du fichier Pending_block à chaque appel de la fonction
}

CellTree* read_tree(){
    int count=0;
    Block *b;
    CellTree *n;
    DIR *repc = opendir("./Blockchain/"); // ouverture du repertoire
    if(repc != NULL){
        struct dirent * dir ;
        while ((dir = readdir(repc))){ // parcours de repertoire afin d'avoir le nombre de block
            if (strcmp(dir->d_name, ".") !=0 && strcmp(dir->d_name, "..") !=0){
                count++;
            }
        }

        closedir(repc);
    }

    CellTree** T = (CellTree **)malloc(sizeof(CellTree *)*count);
    DIR *rep = opendir("./Blockchain/"); // reouverture du repertoire
    chdir("Blockchain/"); //permet de se localiser dans le repertoire afin de manipuler les fichier
    char *tmp;
    if(rep != NULL){
        struct dirent * dir ;
        int i = 0;
        while ((dir = readdir(rep))){ // parcours de repertoire afin de creer les nodes et les ajouter dans T
            if (strcmp(dir->d_name, ".") !=0 && strcmp(dir->d_name, "..") !=0){ //comparaison des noms de fichiers
                // création du block puis du noeud associé
                b=read_file_block(dir->d_name);
                n=create_node(b);
                if(i<count){
                    T[i] = n;
                }
                i++;
            }
        }

        closedir(rep);
    }

    // comparaison du hash et previous_hash afin de reconstituer les arbres
    for(int it=0; it<count; it++){
        for(int jt=0; jt<count; jt++){
            if(strcmp(T[it]->block->hash, T[jt]->block->previous_hash) == 0){
                //print_tree(T[it]);
                add_child(T[it], T[jt]);
            }
        }
    }

    // parcours de T afin d'avoir le noeud racine
    for(int itr=0; itr<count; itr++){
        if(!T[itr]->father){
            return T[itr];
        }
    }
    
    return NULL;
}













