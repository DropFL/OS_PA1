#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MFQ.h"
#include "option.h"

Process** proc_pool;
int n;

int get_index (Process* p) {
    for (int i = 0; i < n; i ++)
        if (proc_pool[i] == p) return i;
}

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

    fscanf(ip, "%d", &n);
    proc_pool = (Process**) malloc(sizeof(Process*) * n);

    for (int i = 0; i < n; i ++) {
        Process* p = proc_pool[i] = read_process(ip);

        if (IS_ACTIVE(p)) {
            VERBOSE printf("Putting this process to queue %d...\n", p->queue_idx);
            p->current_cycle = 1;
            enqueue(mfq->queues[p->queue_idx], p);
        }
        else {
            VERBOSE printf("Putting this process to ready queue...\n");
            enqueue(mfq->ready_queue, p);
        }
    }
    VERBOSE printf("Reading all the processes from %s complete!\n\n", input);

    for (int i = 0; i < n; i ++)
        fprintf(op, "%d ", proc_pool[i]->pid);
    fprintf(op, "\n");

    Process* p;
    int t = 0, dt;
    while (proceed(mfq, &p, &dt)) {
        t += dt;
        NSILENT printf("\n\tElapsed time: %d\n", dt);
        if (p) {
            NSILENT printf("\tProceeded process: %d\n\n", p->pid);
            int idx = get_index(p);

            for (int i = 0; i < dt; i ++) {
                for (int j = 0; j < idx; j ++)
                    fprintf(op, " ");
                fprintf(op, "|\n");
            }

            if (PROC_END(p)) p->term = t;
        }
        else {
            NSILENT printf("\tNo process is proceeded; it was an idle time.\n\n");

            for (int i = 0; i < dt; i ++)
                fprintf(op, "\n");
        }
    }

    float tt_sum = 0, wt_sum = 0;
    for (int i = 0; i < n; i ++) {
        int tt = proc_pool[i]->term - proc_pool[i]->arrival,
            wt = proc_pool[i]->wait;
        
        fprintf(op, "P%d: %d %d\n", proc_pool[i]->pid, tt, wt);
        tt_sum += tt;
        wt_sum += wt;
    }

    fprintf(op, "avg: %.2f %.2f", tt_sum / n, wt_sum / n);

    fclose(ip);
    fclose(op);

    return 0;
}