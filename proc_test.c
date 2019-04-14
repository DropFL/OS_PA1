#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "option.h"

int main (int argc, char *argv[]) {
    if (init(argc, argv)) exit(1);
    VERBOSE { printOptions(); }

    FILE *input = fopen(file, "r");

    if (!input) {
        fprintf(stderr, "failed to open file \"%s\", terminating.\n", file);
        exit(2);
    }

    VERBOSE { printf("file \"%s\" has opened successfully.\n", file); }

    int n;
    fscanf(input, "%d", &n);

    for (int i = 0; i < n; i ++) {
        Process* p = read_process(input);
        if (p) {
            print_process(p);
            free_process(p);
        }
    }

    fclose(input);

    return 0;
}