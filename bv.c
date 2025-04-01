#include "bv.h"
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

//The structure for a Bit Vector.
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};
//Allocates memory for a bit vector..
//Returns a pointer to a bit vector.
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        uint8_t *vector_array = (uint8_t *) calloc(length, sizeof(uint8_t));
        bv->vector = vector_array;
        return bv;
    }
    return NULL;
}
//Frees the memory for a bit vector.
//Sets pointer to NULL.
void bv_delete(BitVector **bv) {
    if (*bv) {
        free((*bv)->vector);
        (*bv)->vector = NULL;
        free(*bv);
        *bv = NULL;
    }
}
//Returns the length of a bit vector, a uint32_t.
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}
//Sets a bit in the bit vector.
//Returns true if bit was set and false if bit was not set.
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] |= (0x1 << (i % 8));
    return true;
}
//Clears a bit in the bit vector.
//Returns true if bit was cleared, false if it wasn't.
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] &= ~(0x1 << (i % 8));
    return true;
}
//Gets the value at an index of the bit vector.
//Returns true if value is 1, false if value is 0.
//Also returns false if index is out of the range.
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    uint8_t temp = bv->vector[i / 8];
    temp &= (0x1 << (i % 8));
    temp = (temp >> (i % 8));
    if (temp == 1) {
        return true;
    }
    return false;
}
//A debugger function that prints the values in a bit vector.
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i += 1) {
        printf("vector[%" PRIu32 "] = %d\n", i, bv_get_bit(bv, i));
    }
}
