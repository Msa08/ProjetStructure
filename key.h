#ifndef KEY_H
#define KEY_H

typedef struct Key{
    long clé;
    long n;
}Key;

void init_key(Key* key, long val, long n);
void init_pair_key(Key* pKey, Key* sKey, long low_size, long up_size);
char* key_to_str(Key* key); 
#endif