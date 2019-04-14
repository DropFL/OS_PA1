#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MFQ.h"
#include "option.h"

int main (int argc, char *argv[]) {
    if (init(argc, argv)) exit(1);
    VERBOSE { printOptions(); }

    FILE *ip = fopen(input, "r");
    FILE *op = fopen(output, "w");

    if (!ip) {
        NSILENT fprintf(stderr, "failed to open file \"%s\", terminating.\n", input);
        exit(2);
    }
    if (!op) {
        NSILENT fprintf(stderr, "failed to open file \"%s\", terminating.\n", output);
        exit(2);
    }

    VERBOSE { printf("file \"%s\" and \"%s\" has opened successfully.\n", input, output); }

    MFQ* mfq = get_mfq(4);
    set_queue(mfq, 0, RR, (void*)2);
    set_queue(mfq, 1, RR, (void*)6);
    set_queue(mfq, 2, SRTN, mfq->ready_queue);
    set_queue(mfq, 3, FCFS, NULL);

    VERBOSE printf("MFQ preset complete!\n\n");

    int n;
    fscanf(ip, "%d", &n);

    for (;n;n--) {
        Process* p = read_process(ip);

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
    VERBOSE printf("Reading all the processes from %s complete!\n\n", input);

    Process* p;
    int t;
    while (proceed(mfq, &p, &t)) {
        NSILENT printf("\n\tElapsed time: %d\n", t);
        if (p) {
            NSILENT printf("\tProceeded process: %d\n\n", p->pid);
            fprintf(op, "P%d %d\n", p->pid, t);

            if (PROC_END(p)) free_process(p);
        }
        else {
            NSILENT printf("\tNo process is proceeded; it was an idle time.\n\n");
            fprintf(op, "IDLE %d\n", t);
        }
    }

    fclose(ip);
    fclose(op);

    return 0;
}