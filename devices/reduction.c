#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>
#include "reduction.h"
#include "../lib/keyspace.h"



static int reduction_length(unsigned long long index);



void 
reduction_init(int k_length, char *tag){

	init_keyspace(k_length, tag);
}



/* reduction function: a different function for each column */
/* transform a sha into a plaintext */
void
sha2plain(unsigned char *sha, int offset, int table, char *plain)
{
	unsigned long long *pUll = &(sha[12]);

	index2plain( (*pUll + offset + table) % get_keyspace(), plain);		
}


/*transform a sha into a index*/
unsigned long long
sha2index(unsigned char *sha, int offset, int table)
{
	unsigned long long *pUll = &(sha[12]);

	return ( *pUll + offset + table) % get_keyspace();		
}


/* transform a index into a plaintext */
void
index2plain(unsigned long long index, char *plain)
{
	int rlength = reduction_length(index);

	unsigned long long ind = index;

	int j;
	for( j = 0 ; j < rlength ; j++ ){
		plain[j] = get_charset()->elements[ind%get_charset()->length];
		ind /= get_charset()->length;
	}

	plain[j] = '\0';
}

/* transform a index into a plaintext optimized 64 bits */
void
index2plain_64(unsigned long long index, char *plain)
{
	int rlength = reduction_length(index);

	unsigned long long ind = index;

	int j, k;
	for( j = 0 ; j < rlength ; j++ ){
		k = ind & 0x3F;
//printf("- %d -\n",k);
		plain[j] = get_charset()->elements[k];
		ind = ind >> 6;
	}

	plain[j] = '\0';
}

static int 
reduction_length(unsigned long long index){
	int i;
	for( i = get_key_length() - 2 ; i >= 0 ; i--){
		if(index >= get_subspace(i))
			return i+2;
	}
	return 1;
}


