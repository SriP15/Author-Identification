#include "node.h"
#include <stdbool.h>
#include <stdint.h>
#include "ht.h"
#include "speck.h"
#include "salts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

//I learned how the hash table functions below worked by attending the lecture on Hashing.

//The definition of this Hash Table structure was provided in the Assignment 7 document
struct HashTable {
    uint64_t salt[2]; // The salt to use for the hash function.
    uint32_t size; // The number of slots in the hash table.
    Node **slots; // The array of hash table items.
};

//The definition of this Hash Table iterator structure was provided in the Assignment 7 document
struct HashTableIterator {
    HashTable *table; // The hash table to iterate over.
    uint32_t slot; // The current slot the iterator is on.
};

//Dynamically allocates memory for a HashTable.
//Returns a pointer to a HashTable
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->slots = (Node **) calloc(size, sizeof(Node *));
        ht->size = size;
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
    }
    return (ht);
}

//Frees the memory allocated for a HashTable.
//Sets the pointer to NULL.
//Returns nothing.
void ht_delete(HashTable **ht) {
    free((*ht)->slots);
    (*ht)->slots = NULL;
    free(*ht);
    *ht = NULL;
}

//Returns size of the hash table, a uint32_t.
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

//Checks to see if a word is in the hash table,
//if it is there, returns the pointer to the corresponding node.
//Else, returns NULL.
Node *ht_lookup(HashTable *ht, char *word) {
    uint32_t index = hash(ht->salt, word) % ht->size;

    for (uint32_t i = 0; i < ht_size(ht); i++) {
        if (ht->slots[index] && (strcmp(ht->slots[index]->word, word) == 0)) {
            return (ht->slots[index]);
        }
        index = (index + 1) % ht->size;
    }
    return NULL;
}

//Inserts a word into a HashTable.
//Returns a pointer to node of word.
//
//If word is already there, increments word count.
Node *ht_insert(HashTable *ht, char *word) {
    Node *node = ht_lookup(ht, word);
    uint32_t count = 0;
    uint32_t index = hash(ht->salt, word) % ht->size;
    if (node) {
        node->count += 1;
        return node;
    } else {
        while (count < ht->size) {
            if (ht->slots[index] == NULL) {
                Node *new_node = node_create(word);
                ht->slots[index] = new_node;
                new_node->count += 1;
                return new_node;
            }
            index = (index + 1) % ht->size;
            count += 1;
        }
    }
    return NULL;
}

//Debugger function to print hash table contents.
//Returns nothing.
void ht_print(HashTable *ht) {
    for (uint32_t x = 0; x < ht->size; x++) {
        if (ht->slots[x] == NULL) {
            ; //printf("NULL\n");
        } else {
            printf(
                "word is %s, word count is %" PRIu32 "\n", ht->slots[x]->word, ht->slots[x]->count);
        }
    }
}

//Allocates Memory for a Hash Table Iterator.
//Returns a pointer to a HashTable iterator, if memory allocated.
HashTableIterator *hti_create(HashTable *ht) {
    HashTableIterator *hti = (HashTableIterator *) malloc(sizeof(HashTableIterator));
    if (hti) {
        hti->slot = 0;
        hti->table = ht;
    }
    return (hti);
}

//Frees the memory for a Hashtable.
//Returns nothing.
//Sets pointer to NULL.
void hti_delete(HashTableIterator **hti) {
    if (*hti) {
        free(*hti);
        *hti = NULL;
    }
}

//Iterates over the entirity HashTable, returng a valid node at a time.
Node *ht_iter(HashTableIterator *hti) {
    Node *current_node;
    while (hti->slot < ht_size(hti->table)) {
        current_node = hti->table->slots[hti->slot];
        hti->slot += 1;
        if (current_node) {
            return current_node;
        }
    }
    return NULL;
}
