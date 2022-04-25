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


    return 0;
}