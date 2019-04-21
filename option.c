#include <stdio.h>
#include <string.h>
#include "option.h"

int verbose;
int silent;
const char* input;
const char* output;

static int streq (const char *s1, const char *s2) {
    return !strcmp(s1, s2);
}

static const char* DEFAULT_INPUT = "input.txt";
static const char* DEFAULT_OUTPUT = "output.txt";

int init (int argc, char *argv[]) {
    input = DEFAULT_INPUT;
    output = DEFAULT_OUTPUT;

    for (int i = 1; i < argc; i ++) {
        if (*argv[i] == '-') {
            if (streq(argv[i], "-v") || streq(argv[i], "--verbose")) {
                if (verbose) fprintf(stderr, "init warning: multiple verbose option is detected.\n");
                verbose = 1;
            }
            else if (streq(argv[i], "-o")) {
                if (output != DEFAULT_OUTPUT) {
                    fprintf(stderr, "init error: output file must be only one.\n");
                    return 2;
                }
                else output = argv[++i];
            }
            else if (streq(argv[i], "-s") || streq(argv[i], "--silent")) {
                if (silent) fprintf(stderr, "init warning: multiple silent option is detected.\n");
                silent = 1;
            }
            else {
                fprintf(stderr, "init warning: unspecified flag \"%s\" is used.\n", argv[i]);
            }
        }
        else if (input != DEFAULT_INPUT) {
            fprintf(stderr, "init error: input file must be only one.\n");
            return 1;
        }
        else input = argv[i];
    }

    return 0;
}

void printOptions () {
    VERBOSE {
        printf("verbose mode is on\n");
        printf("input file: \"%s\"%s\n", input, (input == DEFAULT_INPUT) ? " (default)" : "");
        printf("output file: \"%s\"%s\n", output, (output == DEFAULT_OUTPUT) ? " (default)" : "");
    }
    else NSILENT {
        printf("input file: \"%s\"\n", input);
        printf("output file: \"%s\"\n", output);
    }
}