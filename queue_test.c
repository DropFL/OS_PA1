#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "option.h"

void print (Process* p, void* arg) {
    printf("Process %d\n", p->pid);
}

int main (int argc, char *argv[]) {
    if (init(argc, argv)) exit(1);
    VERBOSE { printOptions(); }

    FILE *input = fopen(file, "r");

    if (!input) {
        fprintf(stderr, "failed to open file \"%s\", terminating.\n", file);
        exit(2);
    }

    VERBOSE { printf("file \"%s\" has opened successfully.\n", file); }

    ProcQueue* q = get_queue(FCFS, NULL);
    int n;
    fscanf(input, "%d", &n);

    for (int i = 0; i < n; i ++)
        enqueue(q, read_process(input));
    
    iterate(q, print, NULL);

    fclose(input);

    return 0;
}