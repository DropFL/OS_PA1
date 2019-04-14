#include "scheduler.h"
#include "queue.h"
#include <stdlib.h>
#include <stdint.h>

static int round_robin (Process* p, void* arg) {
    intptr_t slice = (intptr_t) arg;

    if (CUR_CYCLE(p) < slice)
        slice = CUR_CYCLE(p);

    return (int) slice;
}

static int srtn (Process* p, void* arg) {
    ProcQueue* ready = (ProcQueue *) arg;
    int idx = 0;
    int cycle = CUR_CYCLE(p);
    int time = cycle;

    for (Element* e = ready->head; e && CUR_CYCLE(e->proc) < cycle; e = e->next) {
        Process* r = e->proc;
        if (p->queue_idx == r->queue_idx && cycle - CUR_CYCLE(r) > NEXT_CYCLE(r)) {
            time = CUR_CYCLE(e->proc);
            break;
        }
    }

    return time;
}

static int fcfs (Process* p, void* arg) {
    return CUR_CYCLE(p);
}

Scheduler* get_scheduler (Policy p, void* arg) {
    Scheduler* sch = (Scheduler*) malloc(sizeof(Scheduler));

    switch(p) {
        case RR:
            sch->arg = arg;
            sch->schedule = round_robin;
            break;
        case SRTN:
            sch->arg = arg;
            sch->schedule = srtn;
            break;
        case FCFS:
            sch->schedule = fcfs;
            sch->arg = NULL;
            break;
    }

    return sch;
}