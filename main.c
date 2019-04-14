#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MFQ.h"
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

    MFQ* mfq = get_mfq(4);
    set_queue(mfq, 0, RR, (void*)2);
    set_queue(mfq, 1, RR, (void*)6);
    set_queue(mfq, 2, SRTN, mfq->ready_queue);
    set_queue(mfq, 3, FCFS, NULL);

    VERBOSE printf("MFQ preset complete!\n\n");

    int n;
    fscanf(input, "%d", &n);

    for (;n;n--) {
        Process* p = read_process(input);

        if (CUR_CYCLE(p)) {
            VERBOSE printf("Putting this process to ready queue...\n");
            enqueue(mfq->ready_queue, p);
        }
        else {
            VERBOSE printf("Putting this process to queue %d...\n", p->queue_idx);
            p->current_cycle = 1;
            enqueue(mfq->queues[p->queue_idx], p);
        }
    }
    VERBOSE printf("Reading all the processes from %s complete!\n\n", file);

    Process* p;
    int t;
    while (proceed(mfq, &p, &t)) {
        printf("\n\tElapsed time: %d\n", t);
        if (p)
            printf("\tProceeded process: %d\n\n", p->pid);
        else
            printf("\tNo process is proceeded; it was an idle time\n\n");

        if (PROC_END(p)) free_process(p);
    }

    fclose(input);

    return 0;
}