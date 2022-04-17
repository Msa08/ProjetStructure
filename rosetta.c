#include <openssl/sha.h>
#include <string.h>

const char *s = "Rosetta code";
unsigned char *d = SHA256(s, strlen(s), 0);
int i;
for(i = 0 ; i < SHA256_DIGEST_LENGTH ; i++){
	printf ( "%02x" , d[i]);
}
putchar('\n');
