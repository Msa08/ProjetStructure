
#include <stdio.h>
#include <stdlib.h>
#include "prime_number.h"
#include "chiffrement.h" 
#include "key.h"
#include "Base.h"
#include "hachage.h"
#include "block.h"
#include "blocktree.h"
#include <time.h>
#include <string.h>
#include <openssl/sha.h>

void print_long_vector(long *result, int size){
    printf("Vector: [");
    for(int i=0;i<size;i++){
        printf("%lx \t",result[i]);
    }
    printf("]\n");
}
int main(){
    srand(time(NULL));

/*____________________________TEST PARTIE 1__________________________________________________________
  printf("\n______________________________TEST PARTIE 1___________________________\n");
    // FILE *f = fopen("modpow.txt", "w");//Pour comparer méthodes d'exponentation modulaire
    // FILE *f2=fopen("modpow2.txt", "w");
    // clock_t time_initial=0;
    // clock_t time_final=0;
    // clock_t time_initial2=0;
    // clock_t time_final2=0;
    // double time;
    // double time2;
    long a, b;
    int m,p;


    //Q1.2 Plus grand nombre premier testable en moins de 2 millième de sec?
    // long i;
    // int res=0;
    // int tmp=0;
    // printf("---Plus grand nombre testable avec is_prime_naive() en moins de 0.002s-----\n");
    // for(i=10000; time<0.0020;i++){// tant que time< 2 millième de sec itérer
    //
    //     time_initial=clock();
    //     tmp=is_prime_naive(i);
    //     time_final=clock();
    //     time=(double)(time_final-time_initial)/CLOCKS_PER_SEC;//
    //     if(tmp==1){ // si i premier
    //         res=i;
    //     }
    //     printf("time : %f, nb : %ld, premier= %d\n",time, i,tmp);
    // }
    // printf("Plus grand nombre : %d\n",res);//186763



    //Test de modpow et modpow_naive
    printf("\n---------------Test de modpow_naive et modpow----------------------\n" );
    for(int i=30;i<40;i++){
        m=(int) rand()%i;
        p=(int) rand()%i;
        a=modpow_naive(m,i,p);
        b=modpow(m,i,p);
        printf("%d^%d mod %d ->  modpow_naive: %lu, modpow : %lu\n",m,i,p,a,b);
    }

    // //Q1.5 Comparaison des performances de modpow_naive et modpow
    // //écriture dans les fichiers modpow_naive et modpow
    // for(int i=1;i<50000;i++){ //la puissance varie de 1 à 50 000
    //   //modpow
    //   time_initial=clock();
    //   modpow(10, i, 12);
    //   time_final=clock();
    //   time=(double)(time_final-time_initial)/CLOCKS_PER_SEC;
    //
    //   //modpow_naive
    //   time_initial2=clock();
    //   modpow_naive(10, i, 12);
    //   time_final2=clock();
    //   time2=(double)(time_final2-time_initial2)/CLOCKS_PER_SEC;
    //
    //   fprintf(f,"%d %f\n",i,time);//modpow
    //   fprintf(f2,"%d %f\n",i,time2);//modpow_naive
    //   printf("%i\n",i);
    // }
    // fclose(f);
    // fclose(f2);


    //Generation des clés;
    printf("\n-------------------------Generation de cles------------------------\n");
    //on génère 2 nb premiers de taille comprise entre 3 et 7 bits
    printf("Generation de 2 nombres premiers : ");
     a=random_prime_number(3,7,5000);
     b=random_prime_number(3,7,5000);
    while(a==b){
        b=random_prime_number(3,7,5000);//pour que a et b soient différents
    }
    printf("a=%lu , b=%lu\n",a,b);
    //generation des valeurs des clés associés à  a et b
    long n;
    long u;
    long s;
    generate_key_values(a,b, &n, &s, &u);
    if(u<0){
        long t= (a-1)*(b-1);
        u=u+t;
    }
    //affichage des valeurs des clés en haxadécimal
    printf("cle publique = (%lx, %lx) \n",s,n);
    printf("cle privee = (%lx, %lx) \n",u,n);
    //initialisation des clés
    Key* pKey=malloc(sizeof(Key));
    init_key(pKey, s,n);
    char* str=key_to_str(pKey);
    printf("key_to_str -> %s\n",str);
    Key* sKey=str_to_key("(1273,1f15)");
    //affichage des cles en décimal
    printf("cle publique = %lu,%lu\n",pKey->cle,pKey->n);
    printf("cle privee = %lu,%lu\n",sKey->cle,sKey->n);


    //chiffrement
    printf("\n-----------------Chiffrement/Dechiffrement-------------------------\n");
    printf("Chiffrement \n");
    char mess1[50]="hello";
    int len=strlen(mess1);
    long *crypted=encrypt(mess1,s,n);//message crypté
    printf("Initial message = %s \n",mess1);
    printf("Encoded representation = ");
    print_long_vector(crypted,len);
    //dechiffrement
    printf("Dechiffrement\n");
    char* decoded=decrypt(crypted,len,u,n);//message decrypté
    printf("Decoded = %s\n",decoded);*/


    /*____________________________TEST PARTIE 2______________________________________________________
      printf("\n______________________________TEST PARTIE 2___________________________\n");
    //Testing Init Keys
    printf("\n-----------------Generation d'une paire de cle---------------------\n");
    init_pair_keys(pKey, sKey,3,7);
    printf("pKey: %lx , %lx \n", pKey->cle, pKey->n);
    printf("sKey: %lx , %lx \n", sKey->cle, sKey->n);
    //Testing Key Serialization
    char* chaine = key_to_str(pKey);
    printf("key to str : %s \n", chaine);
    Key* k = str_to_key(chaine);
    free(chaine);
    printf("str to key : %lx , %lx \n", k->cle, k->n);
    free(k);

    //Testing signature
    printf("\n---------------------Generation de signature-----------------------\n");
    //Candidate keys:
    Key* pKeyC = malloc(sizeof(Key));
    Key* sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC,3,7);
    //Declaration:
    char* mess = key_to_str(pKeyC);
    char* pkeycar=key_to_str(pKey);
    printf("%s vote pour %s\n",pkeycar, mess);
    free(pkeycar);
    Signature* sgn = sign(mess, sKey);
    printf("signature : ");
    print_long_vector(sgn->content,sgn->size);
    chaine = signature_to_str(sgn);
    free(sgn->content);
    free(sgn);
    printf("signature to str : %s \n", chaine);
    sgn = str_to_signature(chaine);
    free(chaine);
    printf("str to signature : ");
    print_long_vector(sgn->content,sgn->size);

    //Testing protected:
    printf("\n-------------------Generation de declarations signe----------------\n");
    Protected* pr = init_protected(pKey,mess, sgn);
    //Verification:
    if (verify(pr)){ printf("Signature valide\n");
    }else{
    printf("Signature non valide\n");
    }
    chaine = protected_to_str(pr);
    printf("protected to str : %s\n", chaine);
    pr = str_to_protected(chaine);
    printf("str to protected : %s %s %s\n",key_to_str(pr->pKey),pr->mess,signature_to_str(pr->sgn));

    free(pKey);
    free(sKey);
    free(pKeyC);
    free(sKeyC);
    free(sgn);
    free(pr);
	free(chaine);
    free(mess);
    free(crypted);
    free(decoded);
    free(str);*/
    /*____________________________TEST PARTIE 3______________________________________________________*/
    printf("\n______________________________TEST PARTIE 3___________________________\n");
    generate_random_data(30,5);
    printf("\ngenerate keys\n");
    CellKey* voters=read_public_keys("keys.txt");
    print_list_keys(voters);
    //delete_list_keys(voters);

    printf("\ngenerate candidates\n");
    CellKey* candidates=read_public_keys("candidates.txt");
    print_list_keys(candidates);
    //delete_list_keys(candidates);

    printf("\ngenerate protected\n");
    CellProtected* decl=read_protected("declarations.txt");
    print_list_protected(decl);
    //delete_cell_protected(LCP);


     Key* gagnant = compute_winner(decl,candidates,voters, 5,30);
     printf("gagnant -> %s\n", key_to_str(gagnant));

    /*____________________________TEST PARTIE 4______________________________________________________*/
    printf("\n______________________________TEST PARTIE 4___________________________\n");
    
    printf("hachage avec SHA_256: \n");
    const unsigned char *s= "Rosetta code" ;
    printf("%s\n hachage...\n",s);
    printf("%s \n",hachage_SHA256(s) );
    char *buffer;
    printf("coucou0\n");
    Block *b=lire_block("Block_0");
    printf("coucou0.1\n");
    buffer=block_to_str(b);
    printf("coucou0.2\n");
    printf("block to str :%s\n",buffer);
    printf("coucou0.3\n");
    compute_proof_of_work(b,4);
    int i=verify_block(b,4);
    printf("%d\n",i);
    ecrire_block("Block_0_bis",b);
    delete_block(b);
    printf("%s\n",key_to_str(b->author));

    return 0;
}
    //on obtient 215000363

    /*Problème :
    -la fonction hash_function retourne des valeurs negatives
    -Il y a 5 cle public dans keys.txt
    -Il y a 3 cle public dans candidates.txt
    -Il y a beaucoup plus de 5 declarations dans declarations.txt
    -compute_winner genere une fois sur deux  hc
    
    
    
    Block *b=lire_block("Block_0");
    FILE *resultats_1=fopen("Temps_calcul_Compute_proof_of_work.txt","wa");
    float debut;
    float fin;
    debut=clock();
    fin=clock();
    for (int i=1;i<4;i++){
        debut=clock();
        compute_proof_of_work(b,i);
        fin=clock();
        fprintf(resultats_1,"%d %f\n",i,(float)(fin-debut)/(CLOCKS_PER_SEC));
        printf("%d\n",i);
        printf("%f\n",(fin-debut)/(CLOCKS_PER_SEC));
    }
    if(verify_block(b,3)==1){
        printf("LE  BLOC EST VALIDE !!\n");
    }
    else{
        printf("LE BLOC N'EST PAS VALIDE\n");
    }
     ecrire_block("test_exo7_2",b);
    printf("*************************************\n");
    Block *b2=lire_block("test_exo7_2.txt");
    ecrire_block("test_exo7_3.txt",b2);*/
