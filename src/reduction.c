#include <string.h>
#include <stdio.h>
#include <openssl/sha.h>
#include "reduction.h"
#include "../lib/keyspace.h"



static unsigned int reduction_length(unsigned long long index);



void 
init_reduction(unsigned int keylen, char *charset_types){

	init_keyspace(keylen, charset_types);
}



/* reduction function: a different function for each column */
/* transform a sha into a plaintext */
void
sha2plain(unsigned char *sha, unsigned int offset, unsigned int table, char *plain)
{
	unsigned long long *pUll = &(sha[12]);

	index2plain( (*pUll + offset + table) % get_keyspace(), plain);		
}

/* transform a sha into a plaintext. |Charset| must be equal to 64 */
void
sha2plain_64(unsigned char *sha, unsigned int offset, unsigned int table, char *plain)
{
	unsigned long long *pUll = &(sha[12]);

	index2plain_64( (*pUll + offset + table) % get_keyspace(), plain);		
}


/*transform a sha into a index*/
unsigned long long
sha2index(unsigned char *sha, unsigned int offset, unsigned int table)
{
	unsigned long long *pUll = &(sha[12]);

	return ( *pUll + offset + table) % get_keyspace();		
}

/* transform a index into a plaintext */
void
index2plain(unsigned long long index, char *plain)
{	
	unsigned int rlength = reduction_length(index);

	unsigned long long ind = index;

	unsigned int j;
	for( j = 0 ; j < rlength ; j++ ){
		plain[j] = get_charset()->elements[ind%get_charset()->size];
		ind /= get_charset()->size;
	}

	plain[j] = '\0';
}

/* transform a index into a plaintext. |Charset| must be equal to 64 */
void
index2plain_64(unsigned long long index, char *plain)
{
	unsigned int rlength = reduction_length(index);

	unsigned long long ind = index;

	unsigned int j, k;
	for( j = 0 ; j < rlength ; j++ ){
		k = ind & 0x3F;

		plain[j] = get_charset()->elements[k];
		ind = ind >> 6;
	}

	plain[j] = '\0';
}


/* Returns the length of the word represented by the index */
static unsigned int 
reduction_length(unsigned long long index){

	int i;
	for(i = get_key_length() - 2 ; i >= 0 ; i--){

		if(index >= get_subspace(i))
			return i + 2;
	}
	return 1;
}