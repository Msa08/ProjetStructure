#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "chiffrement.h"
#include "readwrite.h"
#include "compute.h"
#include <math.h>
#include <assert.h>


/* == 4.1 == :
Genere 3 fichiers :
- "KEYS.TXT" contient des couples de cles generes aleatoirement
representant les citoyens (publique, privee)
- "CANDIDATES.TXT " contient les cles publiques de citoyens sele-
ctionnes aleatoirement parmi ceux de "KEYS.TXT" : ce seront les candidats
- "DECLARATIONS.TXT" contient les declarations signees pour chaque citoyen
(avec un candidat choisi au hasard par la liste de candidats)
*/

void generate_random_data(int nv, int nc){

    Key *pKey = NULL;
    Key *sKey = NULL; 

    FILE *f = fopen("Keys.txt", "w");

    // on genere des cles et on ajoute leur representation caractere dans le fichier <keys.txt>
    for (int i=0; i<nv; i++){
        pKey = (Key*)malloc(sizeof(Key));
        sKey = (Key*)malloc(sizeof(Key)); 
        init_pair_keys(pKey, sKey, 3, 7);
        fprintf(f, "%s %s\n", key_to_str(pKey), key_to_str(sKey));
        free(pKey);
        free(sKey);
    }
    fclose(f);

    // RECUPERE les cles publiques dans le fichier Keys 
    // et les ajoute dans le fichier CANDIDATES

    FILE *fk = fopen("Keys.txt", "r");
    FILE *fc = fopen("Candidates.txt", "w");

    //int rand = rand_long(1, nv);
    char **tabCandidats = (char**)malloc(sizeof(char*)*nc);
    for (int i =0; i<nc; i++){
        tabCandidats[i]=malloc(sizeof(char));
    }

    char buffer[100];
    char str[100];
    int it = 0;
    int irand;
    int *tabRand = (int*)malloc(sizeof(int)*nc);
    int itr = 0;
    int estDedans = 0;

    while(it<nc){
        irand = rand() % nv;
        while(itr<it){
            if(tabRand[itr] == irand){
                estDedans++;
                break;
            }
            itr++;
        }
        if(estDedans == 0){
            tabRand[it] = irand; 
            it++;
        }
        itr = 0;
        estDedans = 0;
        irand = 0;

    }

    int tmp;
    for(int i = 0; i < nc; ++i){
        for(int j = i + 1; j < nc; ++j){
            if(tabRand[i] > tabRand[j]){
                tmp = tabRand[i];
                tabRand[i] = tabRand[j];
                tabRand[j] = tmp;
            }
        }
    }
   
    it = 0;
    itr = 0;
    while(fgets(buffer, 100, fk)){
        if(it == tabRand[itr]){
            if (sscanf(buffer, " %s\n", str)==1){
                strcpy(tabCandidats[itr], str);
                fprintf(fc, "%s\n", tabCandidats[itr]);
            }
            itr++;
        }
        it++;
    }

    fclose(fk);
    fclose(fc);

    // GENERE les declarations de vote (a partir de Keys) et les
    // enregistre dans un fichier Declaration.txt

    FILE *fkr = fopen("Keys.txt", "r");
    FILE *fs = fopen("Declarations.txt", "w");
    char str2[100];
    char str3[100];
    while(fgets(buffer, 256, fkr)){
        if (sscanf(buffer, " %s %s \n", str2, str3)==2){
            sKey = str_to_key(str3);
            long rand = rand_long(0, nc-1);
            fprintf(fs, "%s %s %s \n", str2, 
            tabCandidats[rand], 
            signature_to_str(sign(tabCandidats[rand], sKey)));
            free(sKey);
        }
    }
    fclose(fkr);
    fclose(fs);
    
    for(int i=0; i<nc;i++){
        free(tabCandidats[i]);
    }
    free(tabCandidats);
    free(tabRand);  
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){

    // compter le nombre d'elements dans voters et candidates
    // choisir la taille des tables avec size > taille(voters ou candidates)
    HashTable *Hc= create_hashtable(candidates, sizeC);
    HashTable *Hv = create_hashtable(voters, sizeV);

    int estVotant = -1;
    int max = 0;
    Key* GG=(Key*)malloc(sizeof(Key)); 

    // test si la personne est dans Hv à l'aide du champs val
    // test si le message represente bien un candidat dans Hv
    // ++ valeur de la cellule de la table de Hc 
    while(decl){ // parcours de la liste de declarations et compare la liste de votant avec celle des declarations 
        char *cle1 = key_to_str(decl->data->pKey);
        for(int i = 0; i<Hv->size; i++){
            char *cle2 = key_to_str(Hv->tab[i]->key);
            if(strcmp(cle1, cle2) == 0){
                if(Hv->tab[i]->val == 0){
                    Hv->tab[i]->val++; // si il est verifié alors on incremente de 1 
                    estVotant=i; // on garde la position de votant
                    free(cle2);
                    break;
                }
            }
            free(cle2);
        }
        free(cle1);
    
        if(estVotant >=0 && Hv->tab[estVotant]->val == 1){ //1 si il peut voter, 2 si il a voté
            for(int i = 0; i<Hc->size; i++){
                char *cle3 = key_to_str(Hc->tab[i]->key); //recupere le candidat dans la liste de candidats
                if(strcmp(decl->data->mess, cle3) == 0){ // et le cherche dans les declarations
                    Hc->tab[i]->val++;
                    Hv->tab[estVotant]->val++;
                    free(cle3);
                    break;
                }
                free(cle3);
            }
            estVotant = 0;
        }
        decl = decl->next;
    }

    for(int pos = 0; pos<Hc->size; pos++){
        if(Hc->tab[pos]->val > max){
            max = Hc->tab[pos]->val;
            GG = Hc->tab[pos]->key;
        }
    }

    //free Hv et Hc
    delete_hashtable(Hv);
    delete_hashtable(Hc);

    return GG;

}

void compute_proof_of_work(Block *B, int d){
    B->nonce = 0;
    int valide = 0;
    char* block;
    unsigned char *tmp;
    while(B->nonce >= 0){
        block = block_to_str(B);
        tmp = str_to_hash(block);
        for (int i = 0; i<d; i++){
            if(tmp[i] != '0'){
                valide++;
                B->nonce++;
                break;
            }
        }

        if(valide == 0)
            break;
        free(block);
        valide = 0;
    }
    
    /*
    for (int it = 0; it<SHA256_DIGEST_LENGTH; it++)
        printf("%c", tmp[it]);
    putchar('\n');
    */
    
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    CellProtected *liste = fusion_decl(tree);
    delete_fraud_list_protected(&liste);

    return compute_winner(liste, candidates, voters, sizeC, sizeV);
}