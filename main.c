#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MFQ.h"

#define VERBOSE if (verbose)

int streq (const char *s1, const char *s2) { return !strcmp(s1, s2); }

char* const DEFAULT_FILE = "input.txt";
char *file;
int verbose;

void printOptions ();
int init (int argc, char *argv[]);

int main (int argc, char *argv[]) {
    if (init(argc, argv)) exit(1);

    if (verbose) printOptions();

    FILE *input = fopen(file, "r");

    if (!input) {
        fprintf(stderr, "failed to open file \"%s\", terminating.", file);
        exit(2);
    }

    VERBOSE { printf("file \"%s\" has opened successfully.\n", file); }

    fclose(input);

    return 0;
}

void printOptions () {
    printf("===  PA#1 - MFQ Based CPU Scheduler Sim  ===\n");

    // if (verbose) -- this option is on
    printf("verbose mode is on\n");
}

int init (int argc, char *argv[]) {
    for (int i = 1; i < argc; i ++) {
        if (*argv[i] == '-') {
            if (streq(argv[i], "-v") || streq(argv[i], "--verbose")) {
                if (verbose)
                    fprintf(stderr, "warning: multiple verbose option is detected.");
                
                verbose = 1;
            }
        }
        else if (file) {
            fprintf(stderr, "error: input file must be only one.");
            return 1;
        }
        else file = argv[i];
    }

    if (!file) file = DEFAULT_FILE;

    return 0;
}