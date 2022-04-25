#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> 
#include <assert.h>
#include <openssl/sha.h>
#include "chiffrement.h"
#include <dirent.h>
#include "struct.h"
#include "structure.h"
#include "readwrite.h"
#include "compute.h"

/*
PROJET : Blockchain appliquee a un processus electoral

ALALLAH Yassine - n°etu : 28707696
CISSE Ousmane - n°etu : 28711951

LU2IN006 
*/

#define BENCH(X, temps_initial, temps_final, temps_cpu)\
		temps_initial = clock();\
		X;\
		temps_final = clock();\
		temps_cpu=((double)(temps_final - temps_initial))/CLOCKS_PER_SEC\


int main(int argc, char** argv){
    srand(time(NULL));
    
    /*
    int p = 4;
    while(p <= atoi(argv[1]) && temps_cpu<=0.002){
		
		BENCH(is_prime_naive(p), temps_initial, temps_final, temps_cpu);
        if (is_prime_naive(p)==1)
		    printf("Temps : %f et p : %d\n", temps_cpu, p);
		p+=1;
	}
    */

    /*
    clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu;
    
    FILE *fmn = fopen("BENCH_modpow_naive.txt", "w");
    FILE *fm = fopen("BENCH_modpow.txt", "w");

    int max = atoi(argv[1]);
    int m = 0;
    while(m < max){
		
		BENCH(modpow_naive(12,m,10), temps_initial, temps_final, temps_cpu);
        printf("Temps modpow_naive: %f et m : %d\n", temps_cpu, m);
        fprintf(fmn, "%ld %f\n", m, temps_cpu);
        
        BENCH(modpow(12,m,10), temps_initial, temps_final, temps_cpu);
        printf("Temps modpow : %f et m : %d\n", temps_cpu, m);
        fprintf(fm, "%ld %f\n", m, temps_cpu);
		m++;
	}

    fclose(fmn);
    fclose(fm);
    */

printf("================================ Implémentation du protocole RSA ================================\n");
    //Selon la loi de Bernouilli, avec p=3/4 et pour k tirages, on a une probabilité de (1/4)**k
printf("\n");
    //Generation de cle :
    long p = random_prime_number (2, 7, 5000);
    long q = random_prime_number (2, 7, 5000);
    while (p == q){
        q = random_prime_number (2, 7, 5000);
    }
    long n, s, u;
    generate_keys_values (p, q, &n, &s, &u);
    //Pour avoir des cles positives :
    if (u < 0){
        long t = (p-1)*(q-1);
        u = u + t; //on aura toujours s*u mod t = 1
    }
    //Afichage des cles en hexadecimal
    printf ("cle publique  = (%lx,%lx)\n", s, n);
    printf ("cle privee = (%lx,%lx)\n", u, n);

    //Chiffrement:
    char message[10] ="Hello";
    int len = strlen(message);
    long* crypted = encrypt(message, s, n) ;
    printf ("Initial message : %s\n", message);
    printf ("Encoded representation :\n");
    print_long_vector(crypted, len);

    //Dechiffrement
    char* decoded = decrypt(crypted, len, u, n);
    printf ("Decoded : %s \n", decoded);

    free(crypted);
    free(decoded);

printf("\n");
printf("================================ Manipulations de structures sécurisées ================================\n");
printf("\n");
    //Testing Init Keys
    Key *pKey = malloc(sizeof(Key));
    Key *sKey = malloc(sizeof(Key));
    init_pair_keys(pKey, sKey, 3 ,7);
    printf("pKey : %lx, %lx\n", pKey->val, pKey->n);
    printf("sKey : %lx, %lx\n", sKey->val, sKey->n);

    //Testing Key Serialization
    char *chaine = key_to_str(pKey);
    printf("key_to_str : %s \n", chaine);
    Key *k = str_to_key(chaine);
    printf("str_to_key : %lx, %lx \n",k->val, k->n);
    free(k);

    //Testing signature
    //Candidate keys:
    Key *pKeyC = malloc(sizeof(Key));
    Key *sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC, 3 ,7);

    //Declaration:
    char *mess = key_to_str(pKeyC);
    printf(" %s vote pour %s \n", chaine, mess);
    Signature *sgn = sign(mess, sKey);
    printf("signature : ");
    print_long_vector(sgn->content, sgn->size);
    free(chaine);
    chaine = signature_to_str(sgn);
    printf("signature to str : %s \n", chaine);
    sgn = str_to_signature(chaine);
    printf("str to signature : ");
    print_long_vector(sgn->content, sgn->size);
    
    //Testing protected:
    Protected *pr = init_protected(pKey, mess, sgn);
    //Verification:
    if (verify(pr)) {
        printf("Signature valide\n");
    } else {
        printf("Signature non valide\n");
    }
    free(chaine);
    chaine = protected_to_str(pr);
    printf ("protected to str : %s\n", chaine);
    pr = str_to_protected(chaine);
    char *prk = key_to_str(pr->pKey);
    char *prs = signature_to_str(pr->sgn);
    printf("str to protected : %s %s %s\n", prk, pr->mess, prs);

    free(chaine);
    free(mess);
    free(prk);
    free(prs);

printf("\n");
printf("================================ Simulation du processus de vote ================================\n");
printf("\n");
    /*
    clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu;

    FILE *f = fopen("BENCH_compute_PoW.txt", "w");
    Block *block_test = read_file_block("block_test.txt");

    int max = atoi(argv[1]);
    int m = 1;
    while(m < max){
		
		BENCH(compute_proof_of_work(block_test, m), temps_initial, temps_final, temps_cpu);
        printf("Temps compute_PoW: %f et m : %d\n", temps_cpu, m);
        fprintf(f, "%ld %f\n", m, temps_cpu);
        block_test->nonce = 0;
    
		m++;
	}

    fclose(f);
    */

    int sizeV = 1000;
    int sizeC = 5;

    generate_random_data(sizeV, sizeC);
    
    printf("\n");
    printf("Génération des citoyens en cours...\n");
    CellKey *citoyens = read_public_keys("Keys.txt");
    printf("Generation des citoyens terminée\n");

    printf("\n");
    printf("Génération des déclarations en cours...\n");
    CellProtected *declarations = read_protected("Declarations.txt");
    printf("Génération des déclarations terminée\n");

    printf("\n");
    printf("Génération des candidats en cours...\n");
    CellKey *candidats = read_public_keys("Candidates.txt");
    printf("Génération des candidats terminée\n");
    printf("\n");
    
    
    int i = 0;
    int d = 3;
    CellTree *tree = create_node(NULL);
    CellProtected *it = declarations;
    while(it){
        Key* author = it->data->pKey; 
        for(int i =0; i<10; i++){
            submit_vote(it->data);
            printf("Vote ajoutée dans l'urne\n");
            it = it->next;
        }
        printf("\n");
        printf("Création du block %d en cours...\n", i);
        create_block(tree, author, d);
        printf("Création du block %d treminée\n", i);

        char *c = malloc(sizeof(char)*10);
        char *name = malloc(sizeof(char)*256);
        strcpy(name, "Block_");
        sprintf(c, "%d\0", i);
        strcat(name, c);
        printf("\n");
        printf("Vérification du block %d en cours...\n", i);
        add_block(d, name);
        printf("Vérification du block %d treminée\n", i);
        printf("\n");

        free(name);
        free(c);
        i++;  
    }
    
    delete_tree(tree);
    printf("\n");
    printf("Ajout des blocks à la Blockchain en cours...\n");
    tree = read_tree();
    printf("Ajout des blocks à la Blockchain terminée\n");

    printf("\n");
    printf("Affichage de la Blockchain : \n");
    print_tree(tree);
    printf("\n");

    printf("Calcul du gagnant en cours...\n");
    printf("Calcul du gagnant terminée\n");
    printf("\n");
    
    Key *gg = compute_winner_BT(tree, candidats, citoyens, sizeC, sizeV);
    char *gagnant = key_to_str(gg);
    printf("Le gagnant est : %s\n", gagnant);
    free(gagnant);
    free(gg);

    delete_tree(tree);

    delete_cell_key(citoyens);
    delete_cell_key(candidats);
    delete_cell_protected(declarations);
    
    return 0;

}