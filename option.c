#include <stdio.h>
#include <string.h>
#include "option.h"

int verbose;
const char* file;

static int streq (const char *s1, const char *s2) {
    return !strcmp(s1, s2);
}

static const char* DEFAULT_FILE = "input.txt";

int init (int argc, char *argv[]) {
    file = DEFAULT_FILE;

    for (int i = 1; i < argc; i ++) {
        if (*argv[i] == '-') {
            if (streq(argv[i], "-v") || streq(argv[i], "--verbose")) {
                VERBOSE fprintf(stderr, "init warning: multiple verbose option is detected.");
                verbose = 1;
            }
        }
        else if (file) {
            fprintf(stderr, "init error: input file must be only one.");
            return 1;
        }
        else file = argv[i];
    }

    return 0;
}

void printOptions () {
    VERBOSE {
        printf("verbose mode is on\n");
    }


}