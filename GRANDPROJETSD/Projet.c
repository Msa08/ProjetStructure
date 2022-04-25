#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> 
#include <assert.h>
#include <openssl/sha.h>
#include <dirent.h>
//#include <RSA.h>

/*
PROJET : Blockchain appliquee a un processus electoral

CISSE Ousmane - n°etu : 28711951
ALALLAH Yassine - n°etu : 28707696
LU2IN006 

gcc -Wall -o projet Projet.c
*/

#define BENCH(X, temps_initial, temps_final, temps_cpu)\
		temps_initial = clock();\
		X;\
		temps_final = clock();\
		temps_cpu=((double)(temps_final - temps_initial))/CLOCKS_PER_SEC\

/* == 1.1 == :
retourne 1 si p est premier et -1 ou 0 sinon
*/

int is_prime_naive(long p){
    /* 
    p est un entier impair
    ses premieres valeurs sont donc {1,3,5,7...}
    */

    // cas ou p est pair (!= premier)
    if (p%2==0)
        return -1;

    // tous les cas entre 3 et p-1 :
    for(int i=3; i<p;i++){
        if (p%i==0)
            return 0;
    }

    return 1;
}

/* == 1.3 == :
calcule (a^m mod n) de manière naive 
*/

long modpow_naive(long a, long m, long n){
    int i = 0;
    long res=1;

    while (i<m){
        res=(res*a)%n;
        i++;
    }

    return res;

}

/* == 1.4 == :
calcule (a^m mod n) de manière recursive selon la valeur de m 
*/

int modpow(long a, long m, long n){
    long tmp;
    if (m==0)
        return 1;

    if (m%2 == 0){
        tmp = modpow(a, m/2, n);
        return (tmp*tmp)%n;
    }

    if (m%2 != 0){
        tmp = modpow(a, m/2, n);
        return (a*tmp*tmp)%n;
    }
    return 0;
}

/* == 1.6 == :
FONCTIONS PREDEFINIES 
*/
int witness(long a, long b, long d, long p){
    long x = modpow(a, d, p) ;
    if ( x == 1) {
        return 0;
    }
    for (long i = 0; i < b; i ++){
        if(x == p -1){
            return 0;
        }
        x = modpow(x, 2, p);
    }
    return 1;
}

long rand_long(long low, long up){
    return rand() % ( up - low +1)+low;
}

int is_prime_miller(long p, int k){
    if ( p == 2) {
        return 1;
    }
    if (!(p & 1) || p <= 1){ //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1)){ //tant que d n’est pas impair
        d = d/2;
        b = b+1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a ;
    int i ;
    for ( i = 0; i < k ; i ++){
        a = rand_long(2 , p -1);
        if (witness(a,b,d,p)){
            return 0;
        }
    }
    return 1;
}

/* == 1.8 == :
retourne un nombre premier de taille comprise entre low_size et up_size
en effectuant k tests de Miller pour tester la primalite du nombre genere. 
*/


long random_prime_number(int low_size, int up_size, int k){
    int min = pow(2,low_size-1);
    int max = pow(2, up_size)-1;
    long rand = rand_long(min, max);
    int i = is_prime_miller(rand, k);
    while (i!=1){
        rand=rand_long(min, max);
        i = is_prime_miller(rand, k);
    }

    return rand;
}

/* == EXERCICE 2 == :
Version recursive de l'algorithme d'EUCLIDE (fournie) 
*/


long extended_gcd(long s, long t, long *u, long *v){
    if (s==0){
        *u = 0;
        *v = 1;
        return t;
    }

    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u = vPrim-(t/s)*uPrim;
    *v = uPrim;

    return (long)gcd;

}

/* == 2.1 == :
genere les cles publique et secrete en appelant la fonction 
extended_gcd et en mettant a jours n, s et p
*/

void generate_keys_values(long p, long q, long* n, long *s, long *u){
    // calcul de n :
    *n = p*q;
    // calcul de t 
    long t = (p-1)*(q-1);

    // calcul de s :
    long gcd = 0;
    *s = rand_long(0, t);
    long v;
    while (gcd!=1 || *u<0){
        *s = rand_long(0, t);
        gcd = extended_gcd(*s, t, u, &v);
    }

       
}

/* == 2.2 == :
chiffre le message caractere par caractere 
*/

long* encrypt(char * chaine, long s, long n){
    // valeur de retour du message crypte 
    long* tabcrypt=(long *)malloc(sizeof(long)*strlen(chaine));
    int i = 0;

    // cryptage caractere par caractere 
    while(chaine[i] != '\0'){
        tabcrypt[i] = (long)modpow((long)((int)(chaine[i])), s, n);
        i++;
    }
    return tabcrypt;
}

/* == 2.3 == :
dechiffre le message caractere par caractere 
*/

char* decrypt(long * crypted, int size, long  u, long n){
    // chaine de retour 
    char *message=(char*)malloc(sizeof(char)*size+1);

    // cryptage caractere par caractere 
    for(int i=0; i<size; i++){
        message[i]=(char)modpow(crypted[i], u, n);
    }

    message[strlen(message)+1]='\0';
    return message;
}

void print_long_vector(long *result, int size){
    printf ("Vector:[") ;
    for (int i=0; i<size; i++){
        printf("%lx\t",result[i]) ;
    }
    printf ("]\n" ) ;
}

/*=============================== PARTIE 2 ===============================*/

/* == 3.1 == :
structure clé (privée ou publique) contenant 2 long 
*/





/* == 3.7 == :
Cree une signature a partir de la declaration de 
vote (chaine de char de la clé publique du candidat vote)
et d'une cle
*/

Signature* sign(char* mess, Key* sKey){
    // encryptage de la declaration grace a la cle secrete de l'electeur
    long *content = encrypt(mess, sKey->val, sKey->n);
    assert(content);
    // creation et init de la signature 
    Signature *sign = init_signature(content, strlen(mess));
    return sign;
}



/* == 3.9 == :
Declaration de la structure <Protected>
*/

typedef struct _protected{
    Key* pKey;
    char* mess;
    Signature* sgn;
} Protected;







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

/*=============================== PARTIE 3 ===============================*/



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



/*=============================== PARTIE 4 ===============================*/



int hash_function(Key* key, int size){

    char *cle = key_to_str(key);
    float A=(sqrt(5)-1)/2.0;

    int somme = 0;// pour additionner le nombre représentant chaque lettre de la cle
    int i = 0;//index de tableau
    while(cle[i]!='\0'){ //chaque chaîne de caractère finit par '\0'
        somme+=(int)(cle[i]);//cast en int pour avoir le nombre et non la lettre
        i++;
    }

    free(cle);
    return (int)(size*((somme*A)-((int)(somme*A))));

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
/*=============================== PARTIE 5 ===============================*/
typedef struct block {
    Key * author ;
    CellProtected * votes ;
    unsigned char * hash ;
    unsigned char * previous_hash ;
    int nonce ;
} Block;

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

unsigned char *str_to_hash(const char * str){

    unsigned char *d = SHA256(str, strlen(str), 0);
    char *string = malloc(sizeof(char)*(SHA256_DIGEST_LENGTH*2+1));
    int cpt=0;
    for(int it = 0; it<SHA256_DIGEST_LENGTH; it++){
        sprintf(string+cpt, "%02x", d[it]);
        cpt+=2;
    }
    return string;
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
    
    /*for (int it = 0; it<SHA256_DIGEST_LENGTH; it++)
        printf("%c", tmp[it]);
    putchar('\n');
    */
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

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    CellProtected *liste = fusion_decl(tree);
    delete_fraud_list_protected(&liste);

    return compute_winner(liste, candidates, voters, sizeC, sizeV);
}


int main(int argc, char** argv){
    srand(time(NULL));
    
/*================================ MAIN PARTIE 1 ================================*/
printf("================================ MAIN PARTIE 1 ================================\n");
    /*clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu;
    */
    //FILE f = fopen("BENCH.txt");
    /*int p = 4;
    while(p <= atoi(argv[1]) && temps_cpu<=0.002){
		
		BENCH(is_prime_naive(p), temps_initial, temps_final, temps_cpu);
        if (is_prime_naive(p)==1)
		    printf("Temps : %f et p : %d\n", temps_cpu, p);
		p+=1;
	}*/
    /*int max = atoi(argv[1]);
    int m = 0;
    while(m < max){
		
		BENCH(modpow_naive(12,m,10), temps_initial, temps_final, temps_cpu);
        printf("Temps modpow_naive: %f et m : %d\n", temps_cpu, m);
        fprintf("%ld %f\n", m, temps_cpu);
        BENCH(modpow(12,m,10), temps_initial, temps_final, temps_cpu);
        printf("Temps modpow : %f et m : %d\n", temps_cpu, m);
        fprintf("%ld %f\n", m, temps_cpu);
		m++;
	}*/

    //fclose(f);

    //gnuplot

/*================================ MAIN PARTIE 2 ================================*/
printf("================================ MAIN PARTIE 2 ================================\n");
    //Selon la loi de Bernouilli, avec p=3/4 et pour k tirages, on a une probabilité de (1/4)**k

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
        u = u + t ; //on aura toujours s*u mod t = 1
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

/*================================ MAIN PARTIE 3 ================================*/
printf("================================ MAIN PARTIE 3 ================================\n");

    /*
    printf("verify : %d\n", verify(str_to_protected("(2b,55) (2b,55) #50#32#48#e#2a#2a#47#")));
    Signature *sgnnn = sign("(2b,55)", str_to_key("(3,55)"));
    printf(" signature #50#32#48#e#2a#2a#47# : %s\n", signature_to_str(sgnnn));

    printf("verify : %d\n", verify(str_to_protected("(7,9cb) (1139,1963) #597#525#525#7a#62b#b5#525#62b#71d#7a#b1#")));
    Signature *sgnn = sign("(7,9cb)", str_to_key("(2a7,9cb)"));
    printf(" signature #50#32#48#e#2a#2a#47# : %s\n", signature_to_str(sgnn));
    */

    
    
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
    //Testing signature
    //Candidate keys:
    free(k);
    /*
    printf("TEST HASH POS\n");
    for(int i=0; i<15; i++){
        Key *pKeyC = malloc(sizeof(Key));
        Key *sKeyC = malloc(sizeof(Key));
        init_pair_keys(pKeyC, sKeyC, 3 ,7);
        printf("%d\n", hash_function(pKeyC, 15));
        printf("%d\n", hash_function(sKeyC, 15));
        free(pKeyC); free(sKeyC);
    }
    */

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

/*================================ MAIN PARTIE 4 ================================*/
printf("================================ MAIN PARTIE 4 ================================\n");

    int sizeV = 1000;
    int sizeC = 5;

    generate_random_data(sizeV, sizeC);
    
    printf("\n");
    printf("Lecture des citoyens\n");
    CellKey *citoyens = read_public_keys("Keys.txt");
    //print_list_keys(cellkey);
    //delete_list_keys(cellkey);

    printf("\n");
    printf("Lecture des déclarations\n");
    CellProtected *declarations = read_protected("Declarations.txt");
    //afficher_cellPR(cellpr); 

    printf("\n");
    printf("Lecture des citoyens\n");
    CellKey *candidats = read_public_keys("Candidates.txt");
    
    int i = 0;
    int d = 3;
    CellTree *tree = create_node(NULL);
    
    CellProtected *it = declarations;
    while(it){
        Key* author = it->data->pKey; 
        for(int i =0; i<10; i++){
            submit_vote(it->data);
            it = it->next;
        }
        create_block(tree, author, d);

        char *c = malloc(sizeof(char)*10);
        char *name = malloc(sizeof(char)*256);
        strcpy(name, "Block_");
        sprintf(c, "%d\0", i);
        strcat(name, c);

        add_block(d, name);
        free(name);
        free(c);
        i++;  
    }
    free(tree);

    tree = read_tree();
    print_tree(tree);

    Key *gg = compute_winner_BT(tree, candidats, citoyens, sizeC, sizeV);
    char *gagnant = key_to_str(gg);
    printf("le gagnant est : %s\n", gagnant);
    free(gagnant);
    free(gg);

    /*

    
    printf("\n");
    free(pr->mess);
    pr->mess = strdup("(Fraude)");
    printf("\n");

    cellpr = ajout_en_tete_CellPR(cellpr, pr);
    afficher_cellPR(cellpr); 
    
    printf("\n");
    printf("Test functions Fraude_CellProtected\n");
    delete_fraud_list_protected(&cellpr);
 
    delete_list_protected(cellpr);

    free(pKey);
    free(sKey);
    free(pKeyC);
    free(sKeyC);


    
    CellKey *listKey = read_public_keys("Keys.txt");
    //print_list_keys(listKey);
    
    HashTable *table = create_hashtable(listKey, 15);
    /*
    for(int i = 0; i<table->size; i++){
        char *cle = key_to_str((table->tab[i])->key);
        printf("case %d et clé : %s\n", i, cle);
        free(cle);
    }
    */

   /*
    CellKey *voters = read_public_keys("Keys.txt");
    CellKey *candidates = read_public_keys("Candidates.txt");
    CellProtected *decl = read_protected("Declarations.txt");
    int sizeC = 5;
    int sizeV = 15;

    char *gagnant = key_to_str(compute_winner(decl, candidates, voters, sizeC, sizeV));
    printf("le gagnant est : %s\n", gagnant);
    free(gagnant);

    delete_hashtable(table);
    
    const char *sc = "Rosetta code";
    
    unsigned char *d = SHA256(sc, strlen(sc), 0);
    for (int i = 0; i<SHA256_DIGEST_LENGTH; i++)
        printf("%02x", d[i]);
    putchar('\n');
    
    unsigned char *res = str_to_hash(sc);
    printf("test str_to_hash : %s\n", res);
    

    Block *block = read_file_block("Block.txt");
    Block *block1 = read_file_block("Block1.txt");
    Block *block2 = read_file_block("Block.txt");
    Block *block3 = read_file_block("Block.txt");
    Block *block4 = read_file_block("Block.txt");
    Block *block5 = read_file_block("Block.txt");
    
    printf("Compute\n");
    int dit = 2;
    compute_proof_of_work(block, dit);
    printf("%d\n", block->nonce);
    printf("verification = %d\n", verify_block(block, dit));
    
    CellTree *tree = create_node(block);
    tree->block->hash = "tree";
    CellTree *father = create_node(block1);
    father->block->hash = "pere";
    CellTree *nextfather = create_node(block4);
    nextfather->block->hash = "frere pere";
    CellTree *child = create_node(block2); 
    child->block->hash = "child";
    CellTree *nextchild = create_node(block3); 
    nextchild->block->hash = "nextchild";
    CellTree *nextichild = create_node(block5); 
    nextichild->block->hash = "nextichild";

    add_child(father, child);
    add_child(father, nextchild);
    add_child(tree, father);
    add_child(tree, nextfather);
    add_child(child, nextichild);
    
    printf("\n");

    print_tree(tree);
    //delete_tree(tree);

    CellTree* hchild = highest_child(tree);
    printf("Plus grand fils \n");
    print_tree(hchild);

    CellTree* lastnode = last_node(tree);
    printf("Last node\n");
    print_tree(lastnode);

    //à fix
    //CellProtected *listefusion = fusion(tree->block->votes, father->block->votes);
    //afficher_cellPR(listefusion);

    CellProtected *fusion_decl_hichild = fusion_decl(tree);
    afficher_cellPR(fusion_decl_hichild);
    printf("?????????\n");

    submit_vote(fusion_decl_hichild->data);
    create_block(tree, tree->block->author, dit);

    //add_block(dit, "block_valide_ori.txt");
    /*
    DIR *rep = opendir("./Blockchain/");
    if(rep != NULL){
        struct dirent * dir;
        while ((dir = readdir (rep))){
            if (strcmp(dir->d_name, ".") !=0 && strcmp(dir->d_name, "..") !=0){
                printf("Chemin du fichier : ./Blockchain/%s\n", dir->d_name);
            }
        }
        closedir(rep) ;
    }
    */
   /*
   CellTree *node_ori=read_tree();
    if(!node_ori){
        printf("Vide\n");
    }

    print_tree(node_ori);
    
    Key *gg = compute_winner_BT(node_ori, candidates, voters, sizeC, sizeV);
    gagnant = key_to_str(gg);
    printf("le gagnant est : %s\n", gagnant);
    free(gagnant);
    free(gg);

    */
    return 0;   
}

