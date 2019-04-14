#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "option.h"

static void print (Process* p, void* arg) {
    printf("Process %d\n", p->pid);
    printf("%s: Remaining time is %d.\n", arg, CUR_CYCLE(p));
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
    ProcQueue* q2 = get_queue(SRTN, q);
    int n;
    char* tmp;
    fscanf(input, "%d", &n);

    for (int i = 0; i < n; i ++)
        enqueue(q, read_process(input));

    while (!is_empty(q))
        move_head(q, q2);

    move_head(q2, q);
    move_head(q2, q);
    
    iterate(q, print, tmp = strdup("q"));
    free(tmp);
    
    iterate(q2, print, tmp = strdup("q2"));
    free(tmp);

    printf("schedule: %d", schedule(q2));

    fclose(input);

    return 0;
}