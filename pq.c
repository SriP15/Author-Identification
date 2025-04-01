#include <stdio.h>
#include "pq.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdbool.h>
#include <inttypes.h>

//A structure that holds the value of an author and the distance.
typedef struct AuthorsEntry {
    char *author;
    double dist;
} AuthorsEntry;

//The struct for a Priority Queue.
//Contains a uint32_t head value, and uint32_t tail value.
//Contains a uint32_t size value, and uint32_t capacity value.
//Also contains an array of node pointers called items.
struct PriorityQueue {
    uint32_t head;
    uint32_t tail;
    uint32_t capacity;
    uint32_t size;
    AuthorsEntry **items;
};
//Allocates memory for an author entry struct.
//Returns a pointer to an author entry.
AuthorsEntry *ae_create(char *authors, double dist) {
    AuthorsEntry *ae = (AuthorsEntry *) malloc(sizeof(AuthorsEntry));
    if (ae) {
        char *author = strdup(authors);
        ae->author = author;
        ae->dist = dist;
        return ae;
    }
    return ((AuthorsEntry *) NULL);
}

//Frees the memory for an author entry struct
void ae_delete(AuthorsEntry **ae) {
    free(*ae);
    *ae = NULL;
}
//Allocates memory for a Priority Queue.
//Returns a pointer to a priority queue, if it allocated.
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(capacity * sizeof(PriorityQueue));
    if (pq) {
        pq->head = 0;
        pq->tail = 0;
        pq->size = 0;
        pq->capacity = capacity;
        AuthorsEntry **pq_items = (AuthorsEntry **) calloc(capacity, sizeof(AuthorsEntry *));
        pq->items = pq_items;
        return pq;
    }
    return ((PriorityQueue *) NULL);
}
//Frees memory for a priority queue.
void pq_delete(PriorityQueue **q) {
    for (uint32_t i = 0; i < (*q)->capacity; i++) {
        //free((*q)->items[(*q)->tail]->author);
        ae_delete(&((*q)->items[(*q)->tail]));
    }
    free((*q)->items);
    (*q)->items = NULL;
    free(*q);
    *q = NULL;
}
//Returns if a priority queue is empty or not.
//Returns true if priority queue ie empty, false if it isn't.
bool pq_empty(PriorityQueue *q) {
    if (q->head == q->tail) {
        return true;
    } else {
        return false;
    }
}

//Checks if the priority queue is full.
//Returns true if it is, false if it isn't.
bool pq_full(PriorityQueue *q) {
    if (q->head == (q->capacity)) {
        return true;
    } else {
        return false;
    }
}

//Returns the size of a priority queue, a uint32_t.
uint32_t pq_size(PriorityQueue *q) {
    return (q->size);
}

//Enqueues an element in the priority queue.
//Returns true if element has been enqueued, false if it hasn't.
bool enqueue(PriorityQueue *q, char *author, double dist) {
    uint32_t j = pq_size(q) + q->tail;
    uint32_t k = pq_size(q);

    AuthorsEntry *temp = ae_create(author, dist);
    if (!temp) {
        return false;
    }

    if (pq_full(q)) {
        return false;
    }

    if (pq_empty(q)) {
        q->items[q->tail] = temp;
    } else {
        while ((k > 0) && ((temp->dist) < (q->items[j - 1]->dist))) {
            q->items[j] = q->items[j - 1];
            k -= 1;
            j -= 1;
        }
        q->items[j] = temp;
    }
    q->size += 1;
    q->head = (q->head + 1);
    return true;
}
//Dequeues an element from the priority queue at the tail
//Returns true to indicate an element has been dequeued,
//false to indicate it hasn't.
bool dequeue(PriorityQueue *q, char **author, double *dist) {
    if (pq_empty(q)) {
        return false;
    }

    *author = q->items[q->tail]->author;
    *dist = q->items[q->tail]->dist;
    q->tail = (q->tail + 1);
    q->size -= 1;
    return true;
}

//A debugger function that prints the values in a priority queue.
void pq_print(PriorityQueue *q) {
    for (uint32_t i = 0; i < q->size; i += 1) {
        printf("item[%" PRIu32 "] symbol is %s, and frequency is %.2f\n", i, q->items[i]->author,
            q->items[i]->dist);
    }
}
