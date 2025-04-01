#include <stdio.h>
#include "bf.h"
#include <stdlib.h>
#include <inttypes.h>
#include "bv.h"
#include "salts.h"
#include "speck.h"
#include <stdbool.h>
#include <stdint.h>

//Structure for a Bloom Filter.
struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt .
    uint64_t secondary[2]; // Secondary hash function salt .
    uint64_t tertiary[2]; // Tertiary hash function salt .
    BitVector *filter;
};

//Allocates memory for a bloom filter.
//Returns a pointer to a bloom filter, if it is allocated.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;

        BitVector *filter_array = bv_create(size);
        bf->filter = filter_array;
    }
    return bf;
}

//Frees the memory for a bloom filter.
//Sets the pointer to NULL.
void bf_delete(BloomFilter **bf) {
    bv_delete(&((*bf)->filter));
    free(*bf);
    *bf = NULL;
}

//Returns the size of the Bloom Filter, a uint32_t.
uint32_t bf_size(BloomFilter *bf) {
    return (bv_length(bf->filter));
}

//Inserts a word into Bloom Filter.
//Returns nothing.
void bf_insert(BloomFilter *bf, char *word) {
    //Three bloom filter hashes.
    uint32_t prim = hash(bf->primary, word) % bf_size(bf);
    uint32_t sec = hash(bf->secondary, word) % bf_size(bf);
    uint32_t tert = hash(bf->tertiary, word) % bf_size(bf);

    bv_set_bit(bf->filter, prim);
    bv_set_bit(bf->filter, sec);
    bv_set_bit(bf->filter, tert);
}

//Checks to see is a word is in the Bloom Filter.
//Returns true if it is, false if it isn't.
bool bf_probe(BloomFilter *bf, char *word) {
    //Three bloom filter hashes.
    uint32_t prim = hash(bf->primary, word) % bf_size(bf);
    uint32_t sec = hash(bf->secondary, word) % bf_size(bf);
    uint32_t tert = hash(bf->tertiary, word) % bf_size(bf);

    if (bv_get_bit(bf->filter, prim) && bv_get_bit(bf->filter, sec)
        && bv_get_bit(bf->filter, tert)) {
        return true;
    } else {
        return false;
    }
}

//Deubugger function that prints the bloom filter.
//returns void.
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
