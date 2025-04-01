#include "metric.h"
#include "parser.h"
#include "bf.h"
#include "bv.h"
#include "speck.h"
#include "ht.h"
#include "text.h"
#include "node.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <regex.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

//regex expression for parsing words.
#define WORD "[a-zA-Z]+(['-]?[a-zA-Z]+)+"
uint32_t noiselimit; // Number of noise words to filter out.

//Struct for a Text.
struct Text {
    HashTable *ht;
    BloomFilter *bf;
    uint32_t word_count;
};

//Dynamically Allocates memory for a Text structure.
//Return a pointer to a text struct if it was allocated.
//Create a Text for either a noise text or other given text file.
Text *text_create(FILE *infile, Text *noise) {
    Text *text = (Text *) malloc(sizeof(Text));
    if (text) {
        text->word_count = 0;
        text->ht = ht_create(524288);
        text->bf = bf_create(2097152);

        regex_t re;
        regcomp(&re, WORD, REG_EXTENDED);
        char *word = NULL;

        //Iterating over all contents of infile
        while ((word = next_word(infile, &re)) != NULL) {
            uint64_t string_length = strlen(word);
            //make words lower case
            for (uint64_t i = 0; i < string_length; i += 1) {
                word[i] = tolower(word[i]);
            }
            //if we are not creating the noise file execute if statement
            if (noise != NULL) {
                if (!bf_probe(noise->bf, word)) {
                    ht_insert(text->ht, word);
                    bf_insert(text->bf, word);
                    text->word_count += 1;
                } else {
                    if (!ht_lookup(noise->ht, word)) {
                        ht_insert(text->ht, word);
                        bf_insert(text->bf, word);
                        text->word_count += 1;
                    }
                }
            } else {
                if (text->word_count < (noiselimit)) {
                    ht_insert(text->ht, word);
                    bf_insert(text->bf, word);
                    text->word_count += 1;
                } else {
                    break;
                }
            }
        }
        regfree(&re);
        return (text);
    }
    return NULL;
}

//Free memory for a text.
//Returns nothing.
//Sets pointer to NULL afterwards.
void text_delete(Text **text) {
    ht_delete(&((*text)->ht));
    bf_delete(&((*text)->bf));
    free((*text));
    (*text) = NULL;
}

//Returns the distance between two texts depending on the Metric given.
//Returns a double.
double text_dist(Text *text1, Text *text2, Metric metric) {
    double sum = 0;
    double frequency1;
    double frequency2;
    HashTableIterator *hti = hti_create(text2->ht);
    HashTableIterator *hti2 = hti_create(text1->ht);
    Node *n = NULL;
    Node *n2 = NULL;
    //Calculates distance between one text in relation to another
    while ((n = ht_iter(hti)) != NULL) {

        //switch based on the metric.
        frequency1 = text_frequency(text1, n->word);
        frequency2 = text_frequency(text2, n->word);

        switch (metric) {

        case EUCLIDEAN: sum += pow((frequency1 - frequency2), 2); break;
        case MANHATTAN: sum += fabs(frequency1 - frequency2); break;
        case COSINE: sum += (frequency1 * frequency2); break;
        default: fprintf(stderr, "Metric Error.\n");
        }
    }
    //Calculates distance of words not in first text, but in second.
    while ((n2 = ht_iter(hti2)) != NULL) {
        if (!text_contains(text2, n2->word)) {
            frequency1 = text_frequency(text1, n2->word);
            frequency2 = 0; //text_frequency(text2, n2->word);

            //switch based on metric
            switch (metric) {

            case EUCLIDEAN: sum += pow((frequency1 - frequency2), 2); break;
            case MANHATTAN: sum += fabs(frequency1 - frequency2); break;
            case COSINE: sum += (frequency1 * frequency2); break;
            default: fprintf(stderr, "Metric Error.\n");
            }
        }
    }
    //additional calculations needed for Cosine and Euclidean metrics
    if (metric == COSINE) {
        sum = 1 - sum;
    } else if (metric == EUCLIDEAN) {
        sum = sqrt(sum);
    }
    //return the sum
    return sum;
}
//Returns the normalized frequency of a word in a Text.
//Returns a double.
double text_frequency(Text *text, char *word) {
    if (!text_contains(text, word)) {
        return 0;
    } else {
        return (((double) ht_lookup(text->ht, word)->count) / (double) (text->word_count));
    }
}

//Check if Text contains a word.
//Returns true if it does, false if it doesn't.
bool text_contains(Text *text, char *word) {
    if (bf_probe(text->bf, word)) {
        if (ht_lookup(text->ht, word)) {
            return true;
        }
    }
    return false;
}

//Prints contents of text.
//Returns Nothing.
void text_print(Text *text) {
    HashTableIterator *hti = hti_create(text->ht);
    Node *n = NULL;
    while ((n = ht_iter(hti)) != NULL) {
        printf("%s\n", n->word);
    }
}
