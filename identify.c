#include "metric.h"
#include "parser.h"
#include "bf.h"
#include "bv.h"
#include "speck.h"
#include "ht.h"
#include "pq.h"
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
#include <unistd.h>

//Prints the matches: author and distance values
//Returns nothing.
void print_matches(uint32_t i, char *string, double dist) {
    printf("%" PRIu32 ") %s [%.15f]\n", i, string, dist);
}
//Prints the usage message, synopsis.
//Returns nothing.
void usage() {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Identifies the author of some anonymous text passed through stdin.\n"
                    "   Basically computes k-nearest neighbors, where k = 1 by default.\n"
                    "   The number of noise words that are removed is tunable.\n"
                    "   Uses either Euclidean, Manhattan, or cosine distance as metric.\n"

                    "USAGE\n"
                    "   ./identify [OPTIONS]\n"

                    "OPTIONS\n"
                    "   -d database     Database of authors and texts [default: lib.db]\n"
                    "   -k matches      Set number of top matches to display [default: 5]\n"
                    "   -n noisefile    Set file of words to ignore [default: noise.txt]\n"
                    "   -l limit        Set noise word limit [default: 100]\n"
                    "   -e              Set distance metric as Euclidean distance [default]\n"
                    "   -m              Set distance metric as Manhattan distance\n"
                    "   -c              Set distance metric as cosine distance\n"
                    "   -v              Enable verbose printing of program run\n"
                    "   -h              Display program help and usage\n");
}

//Parses command-line options, and computes the distances between a given input file
//and other authors' text files from a database file. Prints the top k matches.
int main(int argc, char **argv) {
    FILE *infile = stdin;
    FILE *data_file = fopen("lib.db", "r");
    FILE *noise_file = fopen("noise.txt", "r");
    FILE *author_file;
    uint32_t matches = 5;
    uint32_t match_count = 0;
    Metric m = EUCLIDEAN;
    uint32_t n = 0;
    uint32_t string_len = 0;
    double dist = 0;
    int opt = 0;

    noiselimit = 100;

    char author_string_holder[150];
    char path_string_holder[150];
    char *string = "string";

    //Parsing command-line options.
    while ((opt = getopt(argc, argv, "d:n:k:l:emch")) != -1) {
        switch (opt) {
        case 'd': data_file = fopen(optarg, "r"); break;
        case 'n': noise_file = fopen(optarg, "r"); break;
        case 'k': matches = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'l': noiselimit = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'e': m = EUCLIDEAN; break;
        case 'm': m = MANHATTAN; break;
        case 'c': m = COSINE; break;
        case 'h': usage(); return (EXIT_FAILURE);
        default: usage(); return (EXIT_FAILURE);
        }
    }
    //create noise and anonymous Texts
    Text *noise = text_create(noise_file, NULL);
    Text *anonymous = text_create(infile, noise);

    fseek(data_file, 0, SEEK_SET);
    fscanf(data_file, "%" SCNu32, &n);
    //Create Priority Queue
    PriorityQueue *pq = pq_create(n);

    fgets(author_string_holder, 151, data_file);
    string_len = strlen(author_string_holder);
    author_string_holder[string_len - 1] = '\0';
    //create Texts for all authors in data base
    for (uint32_t i = 0; i < n; i++) {
        fgets(author_string_holder, 151, data_file);
        string_len = strlen(author_string_holder);
        author_string_holder[string_len - 1] = '\0';

        fgets(path_string_holder, 151, data_file);
        string_len = strlen(path_string_holder);
        path_string_holder[string_len - 1] = '\0';

        author_file = fopen(path_string_holder, "r");

        //Skip file if it can't be read.
        if (!author_file) {
            continue;
        }
        match_count += 1;
        Text *author = text_create(author_file, noise);

        dist = text_dist(anonymous, author, m);

        enqueue(pq, author_string_holder, dist);

        fclose(author_file);
    }
    //Printing results
    printf("Top %" PRIu32 ", metric: %s, noise limit: %" PRIu32 "\n", matches, metric_names[m],
        noiselimit);

    if (pq_size(pq) < matches) {
        //printf("in if state\n");
        matches = pq_size(pq);
    }
    for (uint32_t i = 0; i < matches; i += 1) {
        dequeue(pq, &string, &dist);
        print_matches(i + 1, string, dist);
    }

    //close the files
    fclose(data_file);
    fclose(noise_file);
}
