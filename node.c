#include <stdio.h>
#include <inttypes.h>
#include "node.h"
#include <string.h>
#include <stdlib.h>

//Creating a node structure by dynamically allocating memory for it
//Returns a node, if it was created properly.
Node *node_create(char *word) {
    Node *node = (Node *) malloc(sizeof(Node));
    if (node) {
        char *word_copy = strdup(word);
        node->word = word_copy;
        node->count = 0;
    }
    return node;
}

//Frees memory allocated for a node.
//Sets pointer to Null.
//
//Returns nothing.
void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}

//Debuuger function that prints node values.
//Returns nothing.
void node_print(Node *n) {
    printf("Node word is: %s, Node frequency: %" PRIu32 "\n", n->word, n->count);
}
