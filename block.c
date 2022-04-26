#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Base.h"
#include "key.h"
#include "chiffrement.h"
#include "block.h"
#include <openssl/sha.h>

void ecrire_block(char * filename, Block * block){
    FILE *f = fopen(filename, "w");
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
	

Block* lire_block(char* filename){
    FILE *f = fopen(filename, "r");
    Block *block = (Block *)malloc(sizeof(Block));

    if (block == NULL){
        printf("Erreur lors de l'allocation du block\n");
        exit(EXIT_FAILURE);
    }

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
            block->votes = add_cell_protected(block->votes, pr);

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

    while(block->votes->next){
        droit=add_cell_protected(droit, block->votes->data);
        block->votes=block->votes->next;
        //printf("%s\n",protected_to_str(block->votes->data));
    }
    droit=add_cell_protected(droit, block->votes->data);
    //printf("%s\n",protected_to_str(droit->data));
    block->votes=droit;
    return block;
}



char * block_to_str(Block* b){
	char * res = (char *) malloc(sizeof(char) * 1059 );
	char * author = key_to_str(b->author);
	char * previous_hash = strdup(b->previous_hash);
	char * vote;
    int i=0;

    if(!res){
        printf("probleme d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    for(int j=0;j < strlen(author);j++){
        res[i]=author[j];
        i++;
    }
    res[i]=' ';
    i++;

    for(int j=0; j < strlen(previous_hash); j++){
        res[i]=previous_hash[j];
        i++;
    }
    res[i]=' ';
    i++; 
	
	CellProtected * votes = b->votes;
	while (votes != NULL) {
		vote = protected_to_str(votes->data);
		for(int j=0; j < strlen(vote); j++){
            res[i]=vote[j];
            i++;
        }
        res[i]=' ';
        i++;
        votes=votes->next;
    }
    res[i--]='\0';

	//Libération mémoire

	return res;
}


char * hachage_SHA256(const unsigned char *s){
	unsigned char *d = SHA256(s, strlen((const char*)s),0);
	
	char* tmp = malloc(sizeof(char)*strlen((const char*)d)*2+1);
	
	int i; 
	int cpt=0;
	for(i=0; i < SHA256_DIGEST_LENGTH; i++){     /* Returns the length in bytes of the hash value of the digest. */
		sprintf(&(tmp[cpt]), "%02x", d[i]);
		cpt=cpt+2;
	}
	
	tmp[cpt]='\0';
	return tmp;	
}

void compute_proof_of_work(Block *b, int d)
{
  b->nonce = 0;

  if (b->hash == NULL)
  {
    char *chaine = block_to_str(b);
    b->hash = decrypt_sha(chaine);
    free(chaine);
  }

  unsigned char temp[256];
  unsigned char nonce[128];
  int cpt = 0, i = 0;
  //on va créer une sous chaine et la hachée
  sprintf(nonce, "%d", b->nonce);
  strcpy(temp, b->hash);
  strcat(temp, nonce);
  unsigned char *decrypt = decrypt_sha(temp);

  //on vérifie que la valeur hachée possède bien d 0 successifs
  while (cpt < d)
  {
    if (decrypt[i++] == '0')
    {
      cpt++; //si on trouve un 0 on ajoute 1 au compteur
    }
    else
    //si on ne trouve pas de 0 alors on réinitialise le compteur à 0, on incrémente b->nonce de 1, et on crée une nouvelle sous-chaine
    {
      b->nonce++;
      i = 0;
      cpt = 0;
      free(decrypt);
      sprintf(nonce, "%d", b->nonce);
      strcpy(temp, b->hash);
      strcat(temp, nonce);
      decrypt = decrypt_sha(temp);

    }
  }
  printf("La sous chaine finale qui permet de rendre le block valide : %d %s\n", b->nonce, decrypt);
  free(decrypt);
}

unsigned char *decrypt_sha(const char *chaine){
  unsigned char *d = SHA256(chaine, strlen(chaine), 0);
  unsigned char temp[8];

  //chaine2 sera la chaine que l'on va retourner
  unsigned char *chaine2 = (unsigned char *)malloc(256 * sizeof(unsigned char));

  //on vérifie si pas d'erreurs lors de l'allocation
  if (chaine2 == NULL)
  {
    printf("Erreur lors de l'allocation");
    exit(EXIT_FAILURE);
  }
  //fin de chaine2
  chaine2[0] = '\0';
  //on ajoute chaque caractère de d dans chaine2 au format hexadécimal
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    sprintf(temp, "%02x", d[i]);
    strcat(chaine2, temp);
  }

  return chaine2;
}

int verify_block(Block* b, int d){ 
    char* chaine = block_to_str(b);
    unsigned char* hashage = decrypt_sha(chaine);
    for(int i = 0; i < 4*d; i++){ //on vérifie qu'il y a bien 4d 0 successifs
        if(hashage[i] != 0){
            return 1; //ne rempli pas les conditions du block donc on renvoie 1 (pas valide)
        }
    }
    free(chaine);
    free(hashage);
    return 0; //sinon on renvoie 0 si le block est valide
}

/*int verify_block(Block* b, int d){
    /* Verifie que le bloc b est correct.
    
    char* str_b = block_to_str(b);
    unsigned char* hash_value = hachage_SHA256(str_b);

    for (int i = 0; i < d; i++){
        if (hash_value[i] != '0'){
            free(str_b);
            return 0;
        }
    }
    if ((strcmp((const char*)b->hash, (const char*)hash_value)) != 0) {
        free(str_b);
        return 0;
    }
    free(str_b);
    return 1;
}*/


//qst7.9
void delete_block(Block *b){
	/*supprime un bloc*/
    delete_list_protected(b->votes);
    free(b->author);
    free(b->hash);
    free(b->previous_hash);
    free(b);
    
}




